#ifndef MENU_H
#define MENU_H

#include "../core/Trie.h"
#include "../data/CSVReader.h"
#include "../data/TagIndex.h"
#include "../search/RelevanceScorer.h"
#include "../user/UserData.h"
#include "../user/Recommender.h"

class Menu{
public:
    Menu();

    void iniciar();
private:
    Trie trie;
    CSVReader csvReader;
    TagIndex tagIndex;
    RelevanceScorer scorer;
    UserData userData;
    Recommender recommender;
    vector<Pelicula> peliculas;

    //Pantallas del menu
    void mostrarPantallInicio();
    void menuPrincipal();
    void menuBuscar();
    void menuVerMasTarde();
    void mennuRecomendaciones();
    void mostrarDetallePelicula(int id);

    //utilidades
    void limpiarPantalla();
    void pausar();

};

#endif
