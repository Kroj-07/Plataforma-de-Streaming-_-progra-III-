#ifndef PESOSFIJOSSTRATEGY_H
#define PESOSFIJOSSTRATEGY_H

#include "IScoringStrategy.h"
#include <string>

// ============================================================
// Estrategia concreta (Strategy Pattern): ponderacion por pesos fijos.
//
// Prioridad: frase completa en titulo > termino en titulo > director/casting
//            > genero > presencia en sinopsis.
//
// NOTA DE DISENO: la sinopsis se puntua por PRESENCIA (una vez), no por cada
// aparicion. Antes se sumaba por aparicion, lo que inflaba peliculas cuyo texto
// repetia subcadenas comunes (ej: "man" dentro de "many", "woman", "human"),
// dejandolas por encima de coincidencias exactas de titulo.
// ============================================================
class PesosFijosStrategy : public IScoringStrategy {
public:
    int puntuar(const Pelicula& p,
                const std::vector<std::string>& terminos) const override {
        int score = 0;

        // Reconstruimos la frase completa ("iron man") para premiar el match exacto.
        std::string frase;
        for (const auto& t : terminos) {
            if (t.empty()) continue;
            if (!frase.empty()) frase += ' ';
            frase += t;
        }

        // Bonus fuerte si el titulo contiene la frase COMPLETA (varias palabras).
        if (terminos.size() > 1 && !frase.empty() &&
            p.titulo.find(frase) != std::string::npos) {
            score += 200;
        }

        for (const std::string& term : terminos) {
            if (term.empty()) continue;
            if (p.titulo.find(term)   != std::string::npos) score += 50;
            if (p.director.find(term) != std::string::npos) score += 30;
            if (p.casting.find(term)  != std::string::npos) score += 30;
            if (p.genero.find(term)   != std::string::npos) score += 10;
            if (p.sinopsis.find(term) != std::string::npos) score += 5;  // presencia, no conteo
        }

        score += (p.anio / 100);  // pequeno bonus por recencia
        return score;
    }

    std::string nombre() const override { return "Pesos Fijos"; }
};

#endif
