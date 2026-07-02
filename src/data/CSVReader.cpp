#include "CSVReader.h"
#include "../core/Normalizador.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <future>
#include <thread>
#include "PeliculaFactory.h"


using std::string;
using std::vector;
using std::stringstream;
using std::ifstream;
using std::getline;



string CSVReader::leerCeldaCSV(stringstream& ss) {
    string celda;
    char c;

    if (!(ss >> c)) return "";

    if (c == '"') {
        // Caso A: celda entre comillas (puede contener comas/saltos).
        while (ss.get(c)) {
            if (c == '"') {
                if (ss.peek() == '"') {           // comilla escapada ""
                    ss.get(c);
                    celda += '"';
                } else {
                    break;                          // fin de celda
                }
            } else {
                celda += c;
            }
        }
        if (ss.peek() == ',') ss.get(c);            // consumir coma
    } else {
        ss.putback(c);
        getline(ss, celda, ',');
    }
    return celda;
}

Pelicula CSVReader::parsearLinea(const string& linea, int id) {
    stringstream ss(linea);
    std::vector<std::string> campos;
    campos.reserve(8);
    for (int i = 0; i < 8; ++i) {
        campos.push_back(leerCeldaCSV(ss));
    }
    // PeliculaFactory normaliza los campos; puede lanzar (ej: stoi del año).
    return PeliculaFactory::crearDesdeCSV(campos, id);
}

vector<Pelicula> CSVReader::cargarDatos(const string& rutaArchivo, TagIndex& indexer) {
    vector<Pelicula> peliculas;
    ifstream archivo(rutaArchivo);
    string linea;

    if (!archivo.is_open()) {
        std::cerr << "[CSVReader] No se pudo abrir: " << rutaArchivo << "\n";
        return peliculas;
    }

    getline(archivo, linea);  // saltar encabezados

    int idActual = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        try {
            Pelicula p = parsearLinea(linea, idActual);
            indexer.agregarPelicula(p);
            peliculas.push_back(p);
            ++idActual;
        } catch (...) {
            // Fila corrupta — saltamos sin romper la carga.
            continue;
        }
    }

    return peliculas;
}

vector<Pelicula> CSVReader::cargarDatosParalelo(const string& rutaArchivo, TagIndex& indexer) {
    vector<Pelicula> peliculas;

    // 1. E/S secuencial: leemos todas las lineas crudas a memoria.
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "[CSVReader] No se pudo abrir: " << rutaArchivo << "\n";
        return peliculas;
    }
    string linea;
    getline(archivo, linea);  // saltar encabezados

    vector<string> lineas;
    while (getline(archivo, linea)) {
        if (!linea.empty()) lineas.push_back(std::move(linea));
    }
    if (lineas.empty()) return peliculas;

    // 2. Repartimos las lineas en N bloques (N = nucleos disponibles).
    unsigned int nHilos = std::thread::hardware_concurrency();
    if (nHilos == 0) nHilos = 4;
    const size_t total = lineas.size();
    const size_t tamBloque = (total + nHilos - 1) / nHilos;

    // Cada hilo procesa su bloque y devuelve las peliculas parseadas EN ORDEN
    // (con id temporal). Las filas corruptas se descartan dentro del worker.
    auto worker = [&lineas, total, tamBloque](size_t bloque) {
        vector<Pelicula> parcial;
        size_t inicio = bloque * tamBloque;
        size_t fin = std::min(inicio + tamBloque, total);
        parcial.reserve(fin - inicio);
        for (size_t i = inicio; i < fin; ++i) {
            try {
                parcial.push_back(parsearLinea(lineas[i], 0));  // id temporal
            } catch (...) {
                continue;  // fila corrupta
            }
        }
        return parcial;
    };

    // 3. Lanzamos los hilos con std::async.
    vector<std::future<vector<Pelicula>>> futuros;
    futuros.reserve(nHilos);
    for (unsigned int b = 0; b < nHilos; ++b) {
        futuros.push_back(std::async(std::launch::async, worker, b));
    }

    // 4. Recogemos los resultados EN ORDEN de bloque, reasignamos ids
    //    contiguos (0,1,2,...) y llenamos el TagIndex en un solo hilo.
    peliculas.reserve(total);
    int idActual = 0;
    for (auto& f : futuros) {
        vector<Pelicula> parcial = f.get();
        for (Pelicula& p : parcial) {
            p.id = idActual++;
            indexer.agregarPelicula(p);
            peliculas.push_back(std::move(p));
        }
    }

    return peliculas;
}
