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
    // Lee el CSV limpio y construye el TagIndex en paralelo.
    static std::vector<Pelicula> cargarDatos(const std::string& rutaArchivo, TagIndex& indexer);

private:
    static std::string leerCeldaCSV(std::stringstream& ss);
};

#endif
