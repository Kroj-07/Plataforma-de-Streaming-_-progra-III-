#ifndef PELICULA_H
#define PELICULA_H

#include <string>
#include <vector>
using namespace std;

struct Pelicula {
    int id;               // Generador de lectura (0, 1, 2...)
    int anio;             // Columna A: Release Year
    string titulo;        // Columna B: Title
    string director;      // Columna D: Director
    string casting;       // Columna E: Cast
    string genero;        // Columna F: Genre
    string sinopsis;      // Columna H: Plot
};

#endif