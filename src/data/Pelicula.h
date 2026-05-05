#ifndef PELICULA_H
#define PELICULA_H

#include <string>
#include <vector>
using namespace std;

struct Pelicula {
    int id;
    string titulo;
    string sinopsis;
    string director;
    vector<string> casting;
    vector<string> generos;
    int año;
    string idioma;
};

#endif