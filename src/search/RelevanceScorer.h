#ifndef RELEVANCESCORER_H
#define RELEVANCESCORER_H

#include "../data/Pelicula.h"
#include <vector>
#include <set>
using namespace std;

class RelevanceScorer {
public:
    // Ordena los IDs por relevancia respecto a la consulta
    // Retorna vector ordenado (mayor score primero)
    vector<int> ordenarPorRelevancia(set<int> ids, string consulta, 
                                      vector<Pelicula>& todasLasPeliculas);
    
    // Retorna los top N resultados
    vector<int> topN(vector<int> ordenados, int n = 5);
};

#endif