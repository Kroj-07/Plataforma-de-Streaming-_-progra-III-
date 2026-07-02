#ifndef RELEVANCESCORER_H
#define RELEVANCESCORER_H

#include <vector>
#include <string>
#include <sstream>

// Utilidad de apoyo a la busqueda.
//
// NOTA: el calculo de relevancia (ranking) se implementa con el patron
// Strategy en IScoringStrategy / PesosFijosStrategy. Aqui solo queda la
// utilidad de tokenizacion que comparten el Buscador y las estrategias.
class RelevanceScorer {
public:
    // Convierte una consulta libre en lista de terminos (split por espacios).
    static std::vector<std::string> tokenizar(const std::string& consulta) {
        std::vector<std::string> terminos;
        std::istringstream iss(consulta);
        std::string tok;
        while (iss >> tok) terminos.push_back(tok);
        return terminos;
    }
};

#endif
