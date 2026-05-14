#include "TagIndex.h"
#include <sstream>
#include <algorithm>
#include <cctype>

using std::string;
using std::vector;
using std::stringstream;
using std::getline;

// Limpia y normaliza un tag individual (trim + minusculas).
static string limpiarTag(string s) {
    s.erase(0, s.find_first_not_of(" \t\n\r"));
    if (!s.empty()) s.erase(s.find_last_not_of(" \t\n\r") + 1);
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

void TagIndex::agregarPelicula(const Pelicula& p) {
    // 1. Director (uno solo)
    string dir = limpiarTag(p.director);
    if (!dir.empty() && dir != "unknown") {
        indexDirector[dir].push_back(p.id);
    }

    // 2. Generos (CSV interno separado por comas)
    stringstream ssG(p.genero);
    string itemG;
    while (getline(ssG, itemG, ',')) {
        string gen = limpiarTag(itemG);
        if (!gen.empty() && gen != "unknown") {
            indexGenero[gen].push_back(p.id);
        }
    }

    // 3. Casting (CSV interno separado por comas)
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
    if      (tipo == "director") return indexDirector.count(busqueda) ? indexDirector[busqueda] : vector<int>();
    else if (tipo == "genero")   return indexGenero.count(busqueda)   ? indexGenero[busqueda]   : vector<int>();
    else if (tipo == "casting")  return indexCasting.count(busqueda)  ? indexCasting[busqueda]  : vector<int>();
    return vector<int>();
}
