#include "RelevanceScorer.h"
#include <algorithm>
#include <cmath>

namespace Streaming {

RelevanceScorer::RelevanceScorer() {}

float RelevanceScorer::calcularScore(const std::string& query, 
                                      const std::vector<std::string>& tags_pelicula,
                                      int popularidad) {
    float score = 0.0f;
    
    // Score por coincidencia de tags (60% del peso)
    float tagScore = calcularTagScore(query, tags_pelicula);
    
    // Score por popularidad (40% del peso)
    float popScore = calcularPopularidadScore(popularidad);
    
    score = (tagScore * 0.6f) + (popScore * 0.4f);
    
    return score;
}

float RelevanceScorer::calcularTagScore(const std::string& query, 
                                         const std::vector<std::string>& tags) {
    if (tags.empty()) return 0.0f;
    
    // Convertir query a minúsculas para comparación
    std::string queryLower = query;
    std::transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);
    
    int coincidencias = 0;
    for (const auto& tag : tags) {
        std::string tagLower = tag;
        std::transform(tagLower.begin(), tagLower.end(), tagLower.begin(), ::tolower);
        
        // Coincidencia exacta o parcial
        if (queryLower.find(tagLower) != std::string::npos || 
            tagLower.find(queryLower) != std::string::npos) {
            coincidencias++;
        }
    }
    
    // Normalizar score entre 0 y 1
    float ratio = static_cast<float>(coincidencias) / tags.size();
    return std::min(ratio * 1.5f, 1.0f); // Boost de 1.5x para coincidencias
}

float RelevanceScorer::calcularPopularidadScore(int popularidad) {
    // Normalizar popularidad (asumiendo escala 0-100)
    if (popularidad <= 0) return 0.0f;
    if (popularidad >= 100) return 1.0f;
    
    // Escala logarítmica para suavizar diferencias extremas
    return std::log10(1.0f + popularidad) / std::log10(101.0f);
}

} // namespace Streaming
