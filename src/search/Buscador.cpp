// =====================================================================
// ORQUESTADOR DE BUSQUEDA - Avance Semana 8 (Integrante D)
// Clase puente que conecta el Trie (B) con el TagIndex (B) y el
// RelevanceScorer (B).  En el avance solo se expone la firma; la logica
// de orquestacion completa (combinar texto + tags + ranking + paginacion
// perezosa) se completara en semanas posteriores.
// =====================================================================
#include "Buscador.h"
#include "RelevanceScorer.h"
#include "../core/Normalizador.h"

using std::string;
using std::set;
using std::vector;

Buscador::Buscador(Trie& t, TagIndex& tx, const vector<Pelicula>& peliculas)
    : trie(t), tagIndex(tx), peliculas(peliculas) {}

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
    return RelevanceScorer::ordenarPorRelevancia(ids, peliculas, Normalizador::normalizar(consulta));
}
