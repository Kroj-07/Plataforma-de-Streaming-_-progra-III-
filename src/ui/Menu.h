#ifndef MENU_H
#define MENU_H

#include "../core/Trie.h"
#include "../core/Repository.h"  // <-- NUEVO
#include "../data/CSVReader.h"
#include "../data/TagIndex.h"
#include "../data/Pelicula.h"
#include "../search/Buscador.h"
#include "../user/UserData.h"
#include "../user/Recommender.h"
#include <vector>
#include <string>

class Menu {
public:
    Menu();
    void iniciar();

private:
    Trie trie;
    TagIndex tagIndex;
    Repository<Pelicula> repositorioPeliculas;  // <-- NUEVO
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
    std::vector<Pelicula> cargarDatosSimulados();  // <-- AHORA DEVUELVE VECTOR
    void mostrarFilaPelicula(int indiceDisplay, int idPelicula) const;
};

#endif