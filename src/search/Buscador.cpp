// =====================================================================
// ORQUESTADOR DE BUSQUEDA
// Clase puente que conecta el Trie con el TagIndex y la estrategia de
// ranking (Strategy Pattern). Combina busqueda por texto + ranking.
// =====================================================================
#include "Buscador.h"
#include "RelevanceScorer.h"
#include "PesosFijosStrategy.h"
#include "TFIDFScoringStrategy.h"
#include "../core/Normalizador.h"
#include <algorithm>
#include <utility>

using std::string;
using std::set;
using std::vector;

// Umbral de relevancia: solo se devuelven películas con puntaje > este valor.
// Ajustado empíricamente para filtrar palabras muy comunes como "the".
static constexpr int UMBRAL_RELEVANCIA = 50;

Buscador::Buscador(Trie& t, TagIndex& tx, Repository<Pelicula>& repo)
    : trie(t), tagIndex(tx), repositorio(repo),
      estrategia(std::make_unique<TFIDFScoringStrategy>(repo))  {}

void Buscador::setEstrategia(std::unique_ptr<IScoringStrategy> nueva) {
    if (nueva) estrategia = std::move(nueva);
}

set<int> Buscador::buscarTexto(const string& consulta) {
    if (consulta.empty()) return {};
    string normal = Normalizador::normalizar(consulta);
    return trie.buscarFrase(normal);
}

set<int> Buscador::buscarTag(const string& tipo, const string& valor) {
    auto ids = tagIndex.buscarPorTag(tipo, valor);
    return set<int>(ids.begin(), ids.end());
}

vector<int> Buscador::buscarOrdenado(const string& consulta) {
    auto ids = buscarTexto(consulta);
    if (ids.empty()) return {};

    auto terminos = RelevanceScorer::tokenizar(Normalizador::normalizar(consulta));

    // Decorate-sort-undecorate: calculamos el puntaje de cada pelicula UNA
    // sola vez (usando la estrategia actual) en lugar de recalcularlo en cada
    // comparacion del sort. Pasa de O(n log n) evaluaciones a O(n).
    vector<std::pair<int, int>> conPuntaje;  // (score, id)
    conPuntaje.reserve(ids.size());
    for (int id : ids) {
        int score = estrategia->puntuar(repositorio.getById(id), terminos);
        conPuntaje.push_back({score, id});
    }

    std::sort(conPuntaje.begin(), conPuntaje.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    // Filtrar resultados con puntaje por debajo del umbral de relevancia.
    vector<int> resultado;
    resultado.reserve(conPuntaje.size());
    for (const auto& par : conPuntaje) {
        if (par.first > UMBRAL_RELEVANCIA) {
            resultado.push_back(par.second);
        }
    }
    return resultado;
}