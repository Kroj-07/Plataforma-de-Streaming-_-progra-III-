// Test aislado del Recommender: verifica correccion (matching tokenizado)
// y que no cuelgue con datasets grandes (verificacion de complejidad).
#include "src/user/Recommender.h"
#include <iostream>
#include <chrono>
#include <algorithm>

int main() {
    std::vector<Pelicula> peliculas;

    Pelicula liked;
    liked.id = 0; liked.anio = 2010; liked.titulo = "inception";
    liked.director = "christopher nolan"; liked.casting = "leonardo dicaprio, tom hardy";
    liked.genero = "accion, ciencia ficcion";
    peliculas.push_back(liked);

    // Mismo director -> deberia recomendarse (+5)
    Pelicula p1;
    p1.id = 1; p1.anio = 2014; p1.titulo = "interstellar";
    p1.director = "christopher nolan"; p1.casting = "matthew mcconaughey";
    p1.genero = "drama, ciencia ficcion";
    peliculas.push_back(p1);

    // Mismo genero en ORDEN DISTINTO -> antes NO matcheaba, ahora SI (+3)
    Pelicula p2;
    p2.id = 2; p2.anio = 2015; p2.titulo = "otra pelicula";
    p2.director = "otro director"; p2.casting = "otro actor";
    p2.genero = "ciencia ficcion, accion";  // orden invertido respecto al like
    peliculas.push_back(p2);

    // Sin relacion -> no deberia aparecer
    Pelicula p3;
    p3.id = 3; p3.anio = 2000; p3.titulo = "no relacionada";
    p3.director = "nadie"; p3.casting = "nadie";
    p3.genero = "romance";
    peliculas.push_back(p3);

    Recommender rec;
    std::set<int> likes = {0};

    auto recs = rec.generarRecomendaciones(likes, peliculas, 5);
    std::cout << "Recomendaciones (deberian ser ids 1 y 2, NO el 3): ";
    for (int id : recs) std::cout << id << " ";
    std::cout << "\n";

    bool ok = (recs.size() == 2) &&
              ((recs[0] == 1 && recs[1] == 2) || (recs[0] == 2 && recs[1] == 1)) &&
              std::find(recs.begin(), recs.end(), 3) == recs.end();
    std::cout << (ok ? "TEST TOKENIZACION: PASO\n" : "TEST TOKENIZACION: FALLO\n");

    // --- Prueba de rendimiento con dataset grande (simula el caso real) ---
    std::vector<Pelicula> grande;
    for (int i = 0; i < 35000; ++i) {
        Pelicula p;
        p.id = i; p.anio = 2000 + (i % 20);
        p.titulo = "pelicula " + std::to_string(i);
        p.director = "director " + std::to_string(i % 500);
        p.casting = "actor" + std::to_string(i % 300) + ", actor" + std::to_string((i+1) % 300);
        p.genero = "genero" + std::to_string(i % 20);
        grande.push_back(p);
    }
    std::set<int> likesGrande = {10, 200, 5000, 15000};

    auto t0 = std::chrono::high_resolution_clock::now();
    auto recsGrande = rec.generarRecomendaciones(likesGrande, grande, 5);
    auto t1 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    std::cout << "Recomendaciones sobre 35000 peliculas / 4 likes: " << ms << " ms\n";

    return 0;
}
