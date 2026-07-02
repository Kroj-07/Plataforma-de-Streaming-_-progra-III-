#ifndef BUSCADOR_H
#define BUSCADOR_H

#include "../core/Trie.h"
#include "../core/Repository.h"
#include "../data/TagIndex.h"
#include "../data/Pelicula.h"
#include "IScoringStrategy.h"
#include <set>
#include <vector>
#include <string>
#include <memory>

class Buscador {
public:
    Buscador(Trie& t, TagIndex& tx, Repository<Pelicula>& repo);

    std::set<int> buscarTexto(const std::string& consulta);
    std::set<int> buscarTag(const std::string& tipo, const std::string& valor);
    std::vector<int> buscarOrdenado(const std::string& consulta);

    // Patron Strategy: permite intercambiar el algoritmo de ranking en
    // tiempo de ejecucion (ej: pesos fijos -> TF-IDF) sin tocar el Buscador.
    void setEstrategia(std::unique_ptr<IScoringStrategy> nueva);

private:
    Trie& trie;
    TagIndex& tagIndex;
    Repository<Pelicula>& repositorio;
    std::unique_ptr<IScoringStrategy> estrategia;  // estrategia de ranking actual
};

#endif
