#ifndef TAGINDEX_H
#define TAGINDEX_H

#include "Pelicula.h"
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class TagIndex {
public:
    // Construir índices a partir de un vector de películas
    void construir(vector<Pelicula>& peliculas);
    
    // Buscar por tag específico
    // tipo: "director", "genero", "casting"
    vector<int> buscarPorTag(string tipo, string valor);
    
private:
    unordered_map<string, vector<int>> indiceDirector;
    unordered_map<string, vector<int>> indiceGenero;
    unordered_map<string, vector<int>> indiceCasting;
};

#endif