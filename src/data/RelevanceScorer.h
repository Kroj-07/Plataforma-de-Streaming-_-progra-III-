#ifndef PLATAFORMA_DE_STREAMING___PROGRA_III_RELEVANCESCORER_H
#define PLATAFORMA_DE_STREAMING___PROGRA_III_RELEVANCESCORER_H

#include <string>
#include <vector>
#include <algorithm>
#include "Pelicula.h"

using namespace std;

class RelevanceScorer {
public:
    /**
     * Calcula la puntuación de una película basada en los términos de búsqueda.
     * @param p La película a evaluar.
     * @param terminos El vector de palabras que el usuario buscó.
     * @return Un entero con el puntaje total.
     */
    static int calcularPuntaje(const Pelicula& p, const vector<string>& terminos) {
        int score = 0;

        for (const string& term : terminos) {
            // 1. Prioridad Máxima: Título (+50 puntos por coincidencia)
            if (p.titulo.find(term) != string::npos) {
                score += 50;
            }

            // 2. Prioridad Alta: Director o Casting (+30 puntos)
            if (p.director.find(term) != string::npos || p.casting.find(term) != string::npos) {
                score += 30;
            }
            // 3. Frecuencia en la Sinopsis (+5 puntos por cada vez que aparezca)
            size_t pos = p.sinopsis.find(term, 0);
            while (pos != string::npos) {
                score += 5;
                pos = p.sinopsis.find(term, pos + term.length());
            }
        }
        // 4. Bonus por Recencia (Año de estreno)
        // Las películas más nuevas tienen un pequeño empuje extra
        score += (p.anio / 100);

        return score;
    }

    /**
     * Ordena una lista de IDs de películas de mayor a menor relevancia.
     */
    static void ordenarPorRelevancia(vector<int>& ids, const vector<Pelicula>& todasLasPeliculas, const vector<string>& terminos) {
        sort(ids.begin(), ids.end(), [&](int a, int b) {
            return calcularPuntaje(todasLasPeliculas[a], terminos) >
                   calcularPuntaje(todasLasPeliculas[b], terminos);
        });
    }
};

#endif
#endif //PLATAFORMA_DE_STREAMING___PROGRA_III_RELEVANCESCORER_H