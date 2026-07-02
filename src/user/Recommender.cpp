#include "Recommender.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <unordered_map>

std::set<std::string> Recommender::tokenizarLista(const std::string& csvLista) {
    std::set<std::string> tokens;
    std::stringstream ss(csvLista);
    std::string item;
    while (std::getline(ss, item, ',')) {
        // trim de espacios
        item.erase(0, item.find_first_not_of(" \t\n\r"));
        if (!item.empty()) item.erase(item.find_last_not_of(" \t\n\r") + 1);
        std::transform(item.begin(), item.end(), item.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (!item.empty()) tokens.insert(item);
    }
    return tokens;
}

std::vector<int> Recommender::generarRecomendaciones(
    const std::set<int>& likesIDs,
    const std::vector<Pelicula>& todasLasPeliculas,
    int cantidad)
{
    std::vector<int> recomendados;
    if (likesIDs.empty() || todasLasPeliculas.empty()) return recomendados;

    // 1. Mapa id -> puntero, construido UNA sola vez. O(P).
    //    Evita el find_if repetido dentro del doble loop (antes O(P^2 * L)).
    std::unordered_map<int, const Pelicula*> porId;
    porId.reserve(todasLasPeliculas.size());
    for (const auto& p : todasLasPeliculas) porId[p.id] = &p;

    // 2. Pre-tokenizamos genero/casting de las peliculas likeadas UNA sola vez.
    //    Comparar por interseccion de conjuntos en vez de string exacto:
    //    "accion, drama" y "drama, accion" ahora SI se consideran iguales.
    struct LikeInfo {
        std::string director;
        std::set<std::string> generos;
        std::set<std::string> casting;
    };
    std::vector<LikeInfo> likes;
    likes.reserve(likesIDs.size());
    for (int id : likesIDs) {
        auto it = porId.find(id);
        if (it == porId.end()) continue;
        const Pelicula* p = it->second;
        likes.push_back({p->director, tokenizarLista(p->genero), tokenizarLista(p->casting)});
    }

    auto interseccionNoVacia = [](const std::set<std::string>& a, const std::set<std::string>& b) {
        for (const auto& tok : a) if (b.count(tok)) return true;
        return false;
    };

    // 3. Un solo recorrido O(P), cada pelicula se compara contra L likes ya
    //    pre-procesados (sin volver a buscar en todasLasPeliculas). O(P * L).
    std::vector<std::pair<int, int>> scores; // (id, score)
    for (const auto& p : todasLasPeliculas) {
        if (likesIDs.count(p.id)) continue;

        std::set<std::string> generoP  = tokenizarLista(p.genero);
        std::set<std::string> castingP = tokenizarLista(p.casting);

        int score = 0;
        for (const auto& like : likes) {
            if (!p.director.empty() && p.director == like.director) score += 5;
            if (interseccionNoVacia(generoP, like.generos))        score += 3;
            if (interseccionNoVacia(castingP, like.casting))       score += 2;
        }

        if (score > 0) scores.push_back({p.id, score});
    }

    std::sort(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    for (int i = 0; i < std::min(cantidad, (int)scores.size()); i++) {
        recomendados.push_back(scores[i].first);
    }

    return recomendados;
}
