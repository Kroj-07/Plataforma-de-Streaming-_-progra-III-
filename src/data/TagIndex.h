#ifndef TAGINDEX_H
#define TAGINDEX_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Pelicula.h"

using namespace std;

class TagIndex {
private:
    // Estructuras de búsqueda rápida (O(1) promedio)
    unordered_map<string, vector<int>> indexDirector;
    unordered_map<string, vector<int>> indexGenero;
    unordered_map<string, vector<int>> indexCasting;

public:
    TagIndex() = default;

    /**
     * Agrega una película a los índices de tags.
     * Se debe llamar durante la lectura del CSV.
     */
    void agregarPelicula(const Pelicula& p);

    /**
     * Busca IDs de películas por un tag específico.
     * @param tipo El tipo de tag ("director", "genero", "casting")
     * @param valor El nombre buscado (ej: "Christopher Nolan")
     */
    vector<int> buscarPorTag(const string& tipo, const string& valor);

    // Getters para que el Buscador pueda acceder si es necesario
    const unordered_map<string, vector<int>>& getIndexGenero() const { return indexGenero; }
};

#endif