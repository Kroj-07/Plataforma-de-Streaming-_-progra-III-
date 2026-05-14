#ifndef NORMALIZADOR_H
#define NORMALIZADOR_H

#include <string>
#include <cctype>
#include <algorithm>

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
};

#endif
