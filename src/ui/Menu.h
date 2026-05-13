#ifndef MENU_H
#define MENU_H

#include "../core/Trie.h"
#include "../data/CSVReader.h"
#include "../data/TagIndex.h"
#include "../data/Pelicula.h"
#include "../search/Buscador.h"
#include "../user/UserData.h"
#include "../user/Recommender.h"
#include <vector>
#include <string>

// Punto único de contacto entre la UI y los módulos de A, B y C.
// Integrante D.
class Menu {
public:
    Menu();

    void iniciar();

private:
    // Orden de declaración importa — Buscador depende de Trie, TagIndex, peliculas.
    Trie trie;
    TagIndex tagIndex;
    std::vector<Pelicula> peliculas;
    Buscador buscador;
    UserData userData;
    Recommender recommender;

    // Pantallas
    void pantallaInicio();
    void menuBuscar();
    void menuVerMasTarde();
    void menuRecomendaciones();
    void mostrarDetallePelicula(int id);

    // Utilidades
    void limpiarPantalla();
    void pausar();
    void cargarDatosSimulados();
    void mostrarFilaPelicula(int indiceDisplay, int idPelicula) const;
};

#endif
