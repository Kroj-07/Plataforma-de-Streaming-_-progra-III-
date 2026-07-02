#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "../data/Pelicula.h"
#include <vector>
#include <set>
#include <string>

// Algoritmo de recomendaciones (Integrante C).
// Recibe el conjunto de IDs likeados y la lista completa de películas;
// devuelve hasta `cantidad` IDs recomendados, excluyendo likes previos.
class Recommender {
public:
    std::vector<int> generarRecomendaciones(
        const std::set<int>& likesIDs,
        const std::vector<Pelicula>& todasLasPeliculas,
        int cantidad = 5);

private:
    // Separa una lista tipo "accion, drama" en un conjunto de tokens
    // normalizados ("accion", "drama"). Permite comparar por interseccion
    // de conjuntos en vez de comparar el string completo.
    static std::set<std::string> tokenizarLista(const std::string& csvLista);
};

#endif
