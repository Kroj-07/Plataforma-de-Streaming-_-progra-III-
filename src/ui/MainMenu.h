#ifndef MAINMENU_H
#define MAINMENU_H

#include "MenuBase.h"
#include "IObserver.h"           
#include <string>

class SearchMenu;
class DetailMenu;
class WatchLaterMenu;
class RecommendMenu;

// MainMenu ahora es OBSERVER de UserData
class MainMenu : public MenuBase, public IObserver { 
public:
    MainMenu(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
             Buscador& bus, UserData& ud, Recommender& rec);

    void iniciar();

    // --- Implementación de IObserver ---
    void onDataChanged() override;  

private:
    void mostrarPantallaInicio();
    void ejecutarOpcion(const std::string& opcion);
    std::vector<Pelicula> cargarDatosSimulados();

    // Flag para saber si la pantalla debe actualizarse
    bool pantallaDesactualizada = false;  
};

#endif