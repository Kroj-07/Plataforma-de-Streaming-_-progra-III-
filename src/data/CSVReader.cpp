#include "CSVReader.h"
#include "../core/Normalizador.h"
#include <fstream>
#include <iostream>
#include <algorithm>
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

        stringstream ss(linea);
        Pelicula p;

        try {
            // 1. Recolectamos los 8 campos en un vector
                std::vector<std::string> campos;
                campos.reserve(8);  // Optimización: reservamos espacio para 8 elementos
                for (int i = 0; i < 8; ++i) {
                    campos.push_back(leerCeldaCSV(ss));
}

            // 2. Le pedimos a la fábrica que cree la película con esos campos
            Pelicula p = PeliculaFactory::crearDesdeCSV(campos, idActual);        // H: Plot

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
