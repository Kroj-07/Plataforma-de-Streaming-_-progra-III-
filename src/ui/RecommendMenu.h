#ifndef RECOMMENDMENU_H
#define RECOMMENDMENU_H

#include "MenuBase.h"
#include "DetailMenu.h"
#include <iostream>
#include <vector>
#include <string>

class RecommendMenu : public MenuBase {
public:
    RecommendMenu(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
                  Buscador& bus, UserData& ud, Recommender& rec)
        : MenuBase(t, tx, repo, bus, ud, rec) {}

    void mostrar() {
        limpiarPantalla();
        std::cout << "==========================================\n";
        std::cout << "  RECOMENDACIONES PARA TI\n";
        std::cout << "  (basado en tus likes)\n";
        std::cout << "==========================================\n\n";

        if (userData.getLikes().empty()) {
            std::cout << "Aun no has dado like a ninguna pelicula.\n";
            std::cout << "Busca peliculas y dales like.\n";
            pausar();
            return;
        }

        std::vector<Pelicula> todas = repositorio.getAll();
        auto recs = recommender.generarRecomendaciones(userData.getLikes(), todas, 5);
        if (recs.empty()) {
            std::cout << "No hay recomendaciones disponibles.\n";
            pausar();
            return;
        }

        for (size_t i = 0; i < recs.size(); ++i) {
            mostrarFilaPelicula((int)i + 1, recs[i]);
        }

        std::cout << "------------------------------------------\n";
        std::cout << "Numero para ver detalle, [V] para volver: ";

        std::string opcion;
        std::getline(std::cin, opcion);
        if (opcion.empty() || opcion[0] == 'V' || opcion[0] == 'v') return;

        int indice = leerNumero("Numero para ver detalle: ");
        if (indice >= 1 && indice <= (int)recs.size()) {
            DetailMenu detailMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
            detailMenu.mostrar(recs[indice - 1]);
        } else if (indice != -1) {
            std::cout << "Numero fuera de rango.\n";
        }
        } catch (...) {}
    }
};

#endif