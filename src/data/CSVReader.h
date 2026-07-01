#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include <vector>
#include <sstream>
#include "Pelicula.h"
#include "TagIndex.h"

// Carga y procesamiento de datos (Integrante B).
class CSVReader {
public:
    // Version SECUENCIAL: lee, parsea y normaliza en un solo hilo.
    // Se mantiene para poder comparar tiempos contra la version paralela.
    static std::vector<Pelicula> cargarDatos(const std::string& rutaArchivo, TagIndex& indexer);

    // Version PARALELA: la E/S del archivo es secuencial (una sola pasada),
    // pero el parseo + normalizacion de cada linea se reparte entre varios
    // hilos con std::async. El TagIndex se llena despues, en un solo hilo,
    // porque unordered_map no es thread-safe para escritura concurrente.
    static std::vector<Pelicula> cargarDatosParalelo(const std::string& rutaArchivo, TagIndex& indexer);

private:
    static std::string leerCeldaCSV(std::stringstream& ss);

    // Parsea una unica linea del CSV a un objeto Pelicula.
    // Lanza excepcion si la linea esta corrupta (la maneja quien llama).
    static Pelicula parsearLinea(const std::string& linea, int id);
};

#endif
