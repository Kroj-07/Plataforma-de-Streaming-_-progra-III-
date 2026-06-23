#ifndef WATCHLATERMENU_H
#define WATCHLATERMENU_H

#include "MenuBase.h"
#include "DetailMenu.h"
#include <iostream>
#include <vector>
#include <string>

class WatchLaterMenu : public MenuBase {
public:
    WatchLaterMenu(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
                   Buscador& bus, UserData& ud, Recommender& rec)
        : MenuBase(t, tx, repo, bus, ud, rec) {}

    void mostrar() {
        limpiarPantalla();
        std::cout << "==========================================\n";
        std::cout << "  VER MAS TARDE\n";
        std::cout << "==========================================\n\n";

        const auto& vmt = userData.getVerMasTarde();
        if (vmt.empty()) {
            std::cout << "No tienes peliculas guardadas.\n";
            pausar();
            return;
        }

        std::vector<int> ids(vmt.begin(), vmt.end());
        for (size_t i = 0; i < ids.size(); ++i) {
            mostrarFilaPelicula((int)i + 1, ids[i]);
        }

        std::cout << "------------------------------------------\n";
        std::cout << "Numero para ver detalle, [V] para volver: ";

        std::string opcion;
        std::getline(std::cin, opcion);
        if (opcion.empty() || opcion[0] == 'V' || opcion[0] == 'v') return;

        int indice = leerNumero("Numero para ver detalle: ");
if (indice >= 1 && indice <= (int)ids.size()) {
    DetailMenu detailMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
    detailMenu.mostrar(ids[indice - 1]);
} else if (indice != -1) {
    std::cout << "Numero fuera de rango.\n";
}
    }
};

#endif