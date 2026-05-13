#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "../data/Pelicula.h"
#include <vector>
#include <set>

// Algoritmo de recomendaciones (Integrante C).
// Recibe el conjunto de IDs likeados y la lista completa de películas;
// devuelve hasta `cantidad` IDs recomendados, excluyendo likes previos.
class Recommender {
public:
    std::vector<int> generarRecomendaciones(
        std::set<int> likesIDs,
        std::vector<Pelicula>& todasLasPeliculas,
        int cantidad = 5);
};

#endif
