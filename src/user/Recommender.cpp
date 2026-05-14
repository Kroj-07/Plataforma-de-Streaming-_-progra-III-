#include "Recommender.h"
#include <algorithm>

std::vector<int> Recommender::generarRecomendaciones(
    std::set<int> likesIDs,
    std::vector<Pelicula>& todasLasPeliculas,
    int cantidad)
{
    std::vector<int> recomendados;
    
    if (likesIDs.empty() || todasLasPeliculas.empty()) return recomendados;
    
    // Para cada película no likeada, calcular score
    std::vector<std::pair<int, int>> scores; // (id, score)
    
    for (const auto& p : todasLasPeliculas) {
        if (likesIDs.count(p.id)) continue; // ya tiene like
        
        int score = 0;
        std::string generoP = p.genero;
        std::string directorP = p.director;
        std::string castingP = p.casting;
        
        for (int idLike : likesIDs) {
            // Buscar la película likeada
            auto it = std::find_if(todasLasPeliculas.begin(), todasLasPeliculas.end(),
                [idLike](const Pelicula& peli) { return peli.id == idLike; });
            if (it == todasLasPeliculas.end()) continue;
            
            // Mismo director
            if (directorP == it->director) score += 5;
            
            // Géneros en común (comparación simple)
            if (generoP == it->genero) score += 3;
            
            // Actores en común (comparación simple)
            if (castingP == it->casting && !castingP.empty()) score += 2;
        }
        
        if (score > 0) {
            scores.push_back({p.id, score});
        }
    }
    
    // Ordenar por score descendente
    std::sort(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    // Tomar los primeros 'cantidad'
    for (int i = 0; i < std::min(cantidad, (int)scores.size()); i++) {
        recomendados.push_back(scores[i].first);
    }
    
    return recomendados;
}
