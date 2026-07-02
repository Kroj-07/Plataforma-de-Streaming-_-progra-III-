#ifndef LIKESMENU_H
#define LIKESMENU_H

#include "MenuBase.h"
#include "DetailMenu.h"
#include <iostream>
#include <vector>
#include <string>

// Muestra las peliculas a las que el usuario dio Like, con opcion de abrir
// el detalle (donde puede quitar el like).
class LikesMenu : public MenuBase {
public:
    LikesMenu(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
              Buscador& bus, UserData& ud, Recommender& rec)
        : MenuBase(t, tx, repo, bus, ud, rec) {}

    void mostrar() {
        limpiarPantalla();
        std::cout << "==========================================\n";
        std::cout << "  MIS LIKES\n";
        std::cout << "==========================================\n\n";

        const auto& likes = userData.getLikes();
        if (likes.empty()) {
            std::cout << "Aun no has dado like a ninguna pelicula.\n";
            pausar();
            return;
        }

        std::vector<int> ids(likes.begin(), likes.end());
        for (size_t i = 0; i < ids.size(); ++i) {
            mostrarFilaPelicula((int)i + 1, ids[i]);
        }

        std::cout << "------------------------------------------\n";
        std::cout << "Numero para ver detalle, [V] para volver: ";

        std::string opcion;
        std::getline(std::cin, opcion);
        if (opcion.empty() || opcion[0] == 'V' || opcion[0] == 'v') return;

        int indice;
        try { indice = std::stoi(opcion); } catch (...) { return; }

        if (indice >= 1 && indice <= (int)ids.size()) {
            DetailMenu detailMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
            detailMenu.mostrar(ids[indice - 1]);
        } else {
            std::cout << "Numero fuera de rango.\n";
        }
    }
};

#endif
