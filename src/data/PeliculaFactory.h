#ifndef PELICULAFACTORY_H
#define PELICULAFACTORY_H

// Aquí van los includes necesarios
#include "Pelicula.h"
#include "../core/Normalizador.h"
#include <vector>
#include <string>

// Constantes para los índices de las columnas del CSV
static const int COL_ANIO = 0;
static const int COL_TITULO = 1;
static const int COL_ORIGEN = 2;  // Ignorado
static const int COL_DIRECTOR = 3;
static const int COL_CASTING = 4;
static const int COL_GENERO = 5;
static const int COL_WIKI = 6;    // Ignorado
static const int COL_SINOPSIS = 7;

// Y luego en el código:
p.anio = std::stoi(campos[COL_ANIO]);
p.titulo = Normalizador::normalizar(campos[COL_TITULO]);
// etc.

class PeliculaFactory {
public:
    // Aquí irá el método mágico
    static Pelicula crearDesdeCSV(const std::vector<std::string>& campos, int id) {
    Pelicula p;                   
    p.id = id;                    
    // 3. Asignamos el año (primera columna)
    p.anio = std::stoi(campos[COL_ANIO]); // ¡Cuidado! stoi puede lanzar excepción

    // 4. Asignamos el título (segunda columna) y lo normalizamos
    p.titulo = Normalizador::normalizar(campos[1]);


    // 6. Asignamos director (columna 3)
    p.director = Normalizador::normalizar(campos[3]);

    // 7. Asignamos casting (columna 4)
    p.casting = Normalizador::normalizar(campos[4]);

    // 8. Asignamos género (columna 5)
    p.genero = Normalizador::normalizar(campos[5]);

    // 10. Asignamos sinopsis (columna 7) y normalizamos
    p.sinopsis = Normalizador::normalizar(campos[7]);

    return p; 
}
};

#endif