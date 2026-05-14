// =====================================================================
// ALGORITMO DE RELEVANCIA - Avance Semana 8 (Integrante B)
// Bosquejo inicial del scorer: pondera coincidencias en titulo,
// director/casting y sinopsis con pesos jerarquicos.  La version final
// (incluyendo BM25 o frecuencia inversa) se implementara en semanas
// posteriores, junto con la prueba de ordenamiento contra el dataset
// completo de 34,886 peliculas.
// =====================================================================
#ifndef RELEVANCESCORER_H
#define RELEVANCESCORER_H

#include "../data/Pelicula.h"
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <sstream>

// Algoritmo de relevancia (Integrante B).
// Pondera: título > director/casting > sinopsis.  Bonus por recencia.
class RelevanceScorer {
public:
    // Calcula puntaje de una película respecto a una lista de términos.
    static int calcularPuntaje(const Pelicula& p, const std::vector<std::string>& terminos) {
        int score = 0;
        for (const std::string& term : terminos) {
            if (term.empty()) continue;
            if (p.titulo.find(term)   != std::string::npos) score += 50;
            if (p.director.find(term) != std::string::npos) score += 30;
            if (p.casting.find(term)  != std::string::npos) score += 30;

            size_t pos = p.sinopsis.find(term, 0);
            while (pos != std::string::npos) {
                score += 5;
                pos = p.sinopsis.find(term, pos + term.length());
            }
        }
        score += (p.anio / 100);  // pequeño bonus por recencia
        return score;
    }

    // Convierte una consulta libre en lista de términos (split por espacios).
    static std::vector<std::string> tokenizar(const std::string& consulta) {
        std::vector<std::string> terminos;
        std::istringstream iss(consulta);
        std::string tok;
        while (iss >> tok) terminos.push_back(tok);
        return terminos;
    }

    // Ordena IDs por relevancia (mayor score primero).  O(n log n).
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
};

#endif
