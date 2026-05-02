#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "../data/Pelicula.h"
#include <vector>
#include <set>
using namespace std;

class Recommender {
public:
    // Generar recomendaciones basadas en películas likeadas
    // Excluye películas ya likeadas
    vector<int> generarRecomendaciones(set<int> likesIDs, 
                                         vector<Pelicula>& todasLasPeliculas,
                                         int cantidad = 5);
};

#endif