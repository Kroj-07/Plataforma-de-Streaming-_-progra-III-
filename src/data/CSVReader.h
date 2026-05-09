#ifndef CSVREADER_H
#define CSVREADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Pelicula.h"
#include "TagIndex.h"

using namespace std;

class CSVReader {
public:
    // Retorna un vector con todas las películas y llena el índice de tags
    static vector<Pelicula> cargarDatos(string rutaArchivo, TagIndex& indexer);

private:
    // Función auxiliar para manejar celdas con comillas y comas internas
    static string leerCeldaCSV(stringstream& ss);
};

#endif