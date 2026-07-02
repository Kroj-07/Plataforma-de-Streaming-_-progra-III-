#ifndef RELEVANCESCORER_H
#define RELEVANCESCORER_H

#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <set>
#include <algorithm>
#include "IScoringStrategy.h"
#include "../data/Pelicula.h"

class RelevanceScorer {
private:
    static std::unique_ptr<IScoringStrategy> estrategia;

public:
    // Establecer la estrategia de puntuación (se llama desde MainMenu)
    static void setEstrategia(std::unique_ptr<IScoringStrategy> e) {
        estrategia = std::move(e);
    }

    // Calcular puntaje de una película usando la estrategia activa
    static int calcularPuntaje(const Pelicula& p, const std::vector<std::string>& terminos) {
        if (estrategia) {
            return estrategia->puntuar(p, terminos);
        }
        // Fallback: si no hay estrategia, devolver 0
        return 0;
    }

    // Ordenar IDs por relevancia (mayor puntaje primero)
    static std::vector<int> ordenarPorRelevancia(
            const std::set<int>& ids,
            const std::vector<Pelicula>& peliculas,
            const std::string& consulta) {
        
        auto terminos = tokenizar(consulta);
        std::vector<int> v(ids.begin(), ids.end());
        
        std::sort(v.begin(), v.end(), [&](int a, int b) {
            return calcularPuntaje(peliculas[a], terminos) >
                   calcularPuntaje(peliculas[b], terminos);
        });
        return v;
    }

    // Utilidad de tokenización
    static std::vector<std::string> tokenizar(const std::string& consulta) {
        std::vector<std::string> terminos;
        std::istringstream iss(consulta);
        std::string tok;
        while (iss >> tok) terminos.push_back(tok);
        return terminos;
    }
};

#endif