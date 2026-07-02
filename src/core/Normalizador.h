#ifndef NORMALIZADOR_H
#define NORMALIZADOR_H

#include <string>
#include <cctype>
#include <algorithm>
#include <sstream>

class Normalizador {
public:
    static std::string normalizar(const std::string& texto) {
        std::string resultado;
        for (char c : texto) {
            if (std::isalnum(static_cast<unsigned char>(c)) || c == ' ') {
                resultado += std::tolower(static_cast<unsigned char>(c));
            }
        }
        return resultado;
    }

    // Normaliza una lista separada por comas (casting, genero) preservando
    // las comas como separadores de item, para que TagIndex pueda seguir
    // partiendo el campo por ',' y buscar cada item de forma exacta.
    static std::string normalizarLista(const std::string& texto) {
        std::stringstream ss(texto);
        std::string item;
        std::string resultado;
        bool primero = true;
        while (std::getline(ss, item, ',')) {
            std::string limpio = normalizar(item);
            size_t inicio = limpio.find_first_not_of(' ');
            if (inicio == std::string::npos) continue;
            size_t fin = limpio.find_last_not_of(' ');
            limpio = limpio.substr(inicio, fin - inicio + 1);
            if (limpio.empty()) continue;
            if (!primero) resultado += ", ";
            resultado += limpio;
            primero = false;
        }
        return resultado;
    }
};

#endif
