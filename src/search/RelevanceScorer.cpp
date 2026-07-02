#include "RelevanceScorer.h"

// Inicialización de la variable estática (sin estrategia por defecto)
std::unique_ptr<IScoringStrategy> RelevanceScorer::estrategia = nullptr;