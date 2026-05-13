#ifndef PELICULA_H
#define PELICULA_H

#include <string>
#include <vector>

// Estructura canónica acordada por todos los integrantes.
// IMPORTANTE: usar "anio" (sin ñ) para compatibilidad de encoding.
struct Pelicula {
    int id;                                 // 0, 1, 2... (asignado por CSVReader)
    int anio;                               // Año de estreno
    std::string titulo;                     // Título original
    std::string director;                   // Director principal
    std::string casting;                    // Lista de actores en string
    std::string genero;                     // Géneros separados por coma
    std::string sinopsis;                   // Plot / sinopsis larga
    std::vector<std::string> tags;          // Tags adicionales (opcional)
};

#endif
