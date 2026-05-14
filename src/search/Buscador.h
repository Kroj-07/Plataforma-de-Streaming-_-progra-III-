#ifndef BUSCADOR_H
#define BUSCADOR_H

#include "../core/Trie.h"
#include "../data/TagIndex.h"
#include "../data/Pelicula.h"
#include <set>
#include <vector>
#include <string>

class Buscador {
public:
    Buscador(Trie& t, TagIndex& tx, const std::vector<Pelicula>& peliculas);
    
    std::set<int> buscarTexto(const std::string& consulta);
    std::set<int> buscarTag(const std::string& tipo, const std::string& valor);
    std::vector<int> buscarOrdenado(const std::string& consulta);

private:
    Trie& trie;
    TagIndex& tagIndex;
    const std::vector<Pelicula>& peliculas;
};

#endif
