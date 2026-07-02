#ifndef TFIDFSCORINGSTRATEGY_H
#define TFIDFSCORINGSTRATEGY_H

#include "IScoringStrategy.h"
#include "../data/Pelicula.h"
#include "../core/Repository.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <set>

// ============================================================
// Estrategia concreta (Strategy Pattern): TF-IDF Ponderado.
//
// Mide la relevancia de los términos de búsqueda ponderados por
// el campo en el que aparecen, penalizando términos demasiado
// comunes (como "the", "a", "man") mediante Inverse Document Frequency.
// ============================================================
class TFIDFScoringStrategy : public IScoringStrategy {
private:
    int totalDocumentos;
    std::unordered_map<std::string, int> dfMap; // Frecuencia de documentos (DF) per token

    // Divide un texto en palabras individuales
    std::vector<std::string> tokenizar(const std::string& texto) const {
        std::vector<std::string> tokens;
        std::istringstream iss(texto);
        std::string tok;
        while (iss >> tok) {
            tokens.push_back(tok);
        }
        return tokens;
    }

    // Retorna todos los tokens únicos de una película para calcular DF
    std::set<std::string> obtenerPalabrasUnicas(const Pelicula& p) const {
        std::set<std::string> unicas;
        
        auto t1 = tokenizar(p.titulo);
        auto t2 = tokenizar(p.director);
        auto t3 = tokenizar(p.casting);
        auto t4 = tokenizar(p.genero);
        auto t5 = tokenizar(p.sinopsis);

        unicas.insert(t1.begin(), t1.end());
        unicas.insert(t2.begin(), t2.end());
        unicas.insert(t3.begin(), t3.end());
        unicas.insert(t4.begin(), t4.end());
        unicas.insert(t5.begin(), t5.end());

        return unicas;
    }

    // Cuenta cuántas veces se encuentra una subcadena en un campo
    int contarOcurrencias(const std::string& campo, const std::string& termino) const {
        int count = 0;
        size_t pos = 0;
        while ((pos = campo.find(termino, pos)) != std::string::npos) {
            count++;
            pos += termino.length();
        }
        return count;
    }

public:
    TFIDFScoringStrategy(const Repository<Pelicula>& repo) {
        auto peliculas = repo.getAll();
        totalDocumentos = static_cast<int>(peliculas.size());
        if (totalDocumentos == 0) totalDocumentos = 1;

        // Construir el mapa de Document Frequency (DF)
        for (const auto& p : peliculas) {
            auto palabras = obtenerPalabrasUnicas(p);
            for (const auto& palabra : palabras) {
                dfMap[palabra]++;
            }
        }
    }

    int puntuar(const Pelicula& p, const std::vector<std::string>& terminos) const override {
        double scoreTotal = 0.0;

        // Reconstruimos la frase completa para premiar el match de frase
        std::string frase;
        for (const auto& t : terminos) {
            if (t.empty()) continue;
            if (!frase.empty()) frase += ' ';
            frase += t;
        }

        // Bonus fuerte si el título contiene la frase completa exacta
        if (terminos.size() > 1 && !frase.empty() && p.titulo.find(frase) != std::string::npos) {
            scoreTotal += 500.0; 
        }

        for (const auto& term : terminos) {
            if (term.empty()) continue;

            // 1. Obtener DF y calcular IDF (con suavizado de Laplace)
            int df = 0;
            auto it = dfMap.find(term);
            if (it != dfMap.end()) {
                df = it->second;
            }
            double idf = std::log10(1.0 + (static_cast<double>(totalDocumentos) / (1.0 + df)));

            // 2. Calcular Term Frequency (TF) ponderado por importancia de campo
            double tf = 0.0;
            tf += 10.0 * contarOcurrencias(p.titulo, term);  // Título muy importante
            tf += 5.0  * contarOcurrencias(p.director, term); // Director
            tf += 4.0  * contarOcurrencias(p.casting, term);  // Casting
            tf += 2.0  * contarOcurrencias(p.genero, term);   // Género
            
            // La sinopsis se califica por presencia simple para evitar inflar textos largos
            if (p.sinopsis.find(term) != std::string::npos) {
                tf += 1.0;
            }

            scoreTotal += tf * idf;
        }

        // Escalar la puntuación flotante a un entero (multiplicado por 100)
        int scoreFinal = static_cast<int>(scoreTotal * 100.0);
        
        // Mantener un plus de recencia menor para favorecer películas modernas en caso de empates
        scoreFinal += (p.anio / 100);

        return scoreFinal;
    }

    std::string nombre() const override {
        return "TF-IDF Ponderado";
    }
};

#endif
