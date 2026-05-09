#include "TagIndex.h"
#include <sstream>
#include <algorithm>

// Función auxiliar para limpiar espacios y normalizar cada tag individual
string limpiarTag(string s) {
    // Eliminar espacios al inicio y al final
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    s.erase(s.find_last_not_of(" \t\n\r") + 1);

    // Todo a minúsculas para que "Action" y "action" sean lo mismo
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void TagIndex::agregarPelicula(const Pelicula& p) {
    // 1. Indexar Director (Suele ser uno solo, pero lo limpiamos)
    string dir = limpiarTag(p.director);
    if (!dir.empty() && dir != "unknown") {
        indexDirector[dir].push_back(p.id);
    }

    // 2. Indexar Géneros (Pueden venir como "comedy, drama")
    stringstream ssG(p.genero);
    string itemG;
    while (getline(ssG, itemG, ',')) {
        string gen = limpiarTag(itemG);
        if (!gen.empty() && gen != "unknown") {
            indexGenero[gen].push_back(p.id);
        }
    }

    // 3. Indexar Casting (Pueden venir como "actor 1, actor 2")
    stringstream ssC(p.casting);
    string itemC;
    while (getline(ssC, itemC, ',')) {
        string actor = limpiarTag(itemC);
        if (!actor.empty() && actor != "unknown") {
            indexCasting[actor].push_back(p.id);
        }
    }
}

vector<int> TagIndex::buscarPorTag(const string& tipo, const string& valor) {
    string busqueda = limpiarTag(valor);

    if (tipo == "director") {
        return indexDirector.count(busqueda) ? indexDirector[busqueda] : vector<int>();
    }
    else if (tipo == "genero") {
        return indexGenero.count(busqueda) ? indexGenero[busqueda] : vector<int>();
    }
    else if (tipo == "casting") {
        return indexCasting.count(busqueda) ? indexCasting[busqueda] : vector<int>();
    }

    return vector<int>();
}