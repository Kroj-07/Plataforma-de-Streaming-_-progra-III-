#ifndef CSVREADER_H
#define CSVREADER_H

#include "Pelicula.h"
#include "../core/Trie.h"
#include <vector>
#include <string>
using namespace std;

class CSVReader {
public:
    // Cargar películas desde archivo CSV
    vector<Pelicula> cargarCSV(string rutaArchivo);
    
    // Cargar CSV e insertar automáticamente en el Trie
    vector<Pelicula> cargarCSVeInsertarEnTrie(string rutaArchivo, Trie& trie);
    
private:
    string normalizarTexto(string texto);
    vector<string> tokenizar(string texto);
};

#endif