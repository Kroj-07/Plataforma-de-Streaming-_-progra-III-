#ifndef ISCORINGSTRATEGY_H
#define ISCORINGSTRATEGY_H

#include "../data/Pelicula.h"
#include <vector>
#include <string>

// ============================================================
// PATRON STRATEGY
// ============================================================
// Interfaz que define el "contrato" para calcular la relevancia de una
// pelicula respecto a una lista de terminos de busqueda.
//
// Permite intercambiar el algoritmo de ranking (pesos fijos, TF-IDF, etc.)
// SIN modificar al Buscador (principio Abierto/Cerrado de SOLID).
// ============================================================
class IScoringStrategy {
public:
    virtual ~IScoringStrategy() = default;

    // Devuelve el puntaje de relevancia de 'p' para los 'terminos' dados.
    virtual int puntuar(const Pelicula& p,
                        const std::vector<std::string>& terminos) const = 0;

    // Nombre legible de la estrategia (util para mostrar en la UI / debug).
    virtual std::string nombre() const = 0;
};

#endif
