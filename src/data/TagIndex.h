#ifndef TAGINDEX_H
#define TAGINDEX_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Pelicula.h"

// Índice por tags (Integrante B).
// Acceso O(1) promedio por director, género o actor.
class TagIndex {
private:
    std::unordered_map<std::string, std::vector<int>> indexDirector;
    std::unordered_map<std::string, std::vector<int>> indexGenero;
    std::unordered_map<std::string, std::vector<int>> indexCasting;

public:
    TagIndex() = default;

    // Llamar durante la lectura del CSV.
    void agregarPelicula(const Pelicula& p);

    // tipo: "director" | "genero" | "casting"
    std::vector<int> buscarPorTag(const std::string& tipo, const std::string& valor);

    const std::unordered_map<std::string, std::vector<int>>& getIndexGenero() const {
        return indexGenero;
    }
};

#endif
