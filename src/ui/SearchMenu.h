#ifndef SEARCHMENU_H
#define SEARCHMENU_H

#include "MenuBase.h"
#include "Paginador.h"
#include "DetailMenu.h"
#include <iostream>
#include <string>
#include <vector>

class SearchMenu : public MenuBase {
public:
    SearchMenu(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
               Buscador& bus, UserData& ud, Recommender& rec)
        : MenuBase(t, tx, repo, bus, ud, rec) {}

    void mostrar() {
        limpiarPantalla();
        std::cout << "==========================================\n";
        std::cout << "  BUSCAR PELICULA\n";
        std::cout << "==========================================\n";
        std::cout << "Puede buscar por palabra, frase o parte de una palabra.\n";
        std::cout << "Ejemplos: 'iron', 'barco', 'bat', 'nolan'\n\n";
        std::cout << "Ingrese su busqueda: ";

        std::string consulta;
        std::getline(std::cin, consulta);
        if (consulta.empty()) return;

        std::vector<int> resultados = buscador.buscarOrdenado(consulta);

        if (resultados.empty()) {
            std::cout << "\nNo se encontraron coincidencias para: \"" << consulta << "\"\n";
            pausar();
            return;
        }

        mostrarResultados(consulta, resultados);
    }

private:
    void mostrarResultados(const std::string& consulta, const std::vector<int>& ids) {
        Paginador pag((int)ids.size(), 5);
        char nav = 0;

        do {
            limpiarPantalla();
            std::cout << "==========================================\n";
            std::cout << "  Resultados para: \"" << consulta << "\"\n";
            std::cout << "==========================================\n\n";

            for (int i = pag.getInicio(); i < pag.getFin(); ++i) {
                mostrarFilaPelicula(i - pag.getInicio() + 1, ids[i]);
            }

            std::cout << "------------------------------------------\n";
            std::cout << "  Pagina " << pag.getPaginaActual() << " de " << pag.getTotalPaginas();
            std::cout << "  (Mostrando " << (pag.getFin() - pag.getInicio())
                      << " de " << pag.getTotalResultados() << ")\n";
            std::cout << "------------------------------------------\n";
            std::cout << "  [1-5] Ver detalle  ";
            if (pag.hayAnterior())  std::cout << "[A] Anterior  ";
            if (pag.haySiguiente()) std::cout << "[S] Siguiente  ";
            std::cout << "[V] Volver\n";
            std::cout << "Seleccione: ";

            std::string entrada;
            std::getline(std::cin, entrada);
            if (entrada.empty()) { nav = 0; continue; }
            nav = entrada[0];

            if (nav >= '1' && nav <= '5') {
                int indice = pag.getInicio() + (nav - '1');
                if (indice < pag.getTotalResultados()) {
                    DetailMenu detailMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
                    detailMenu.mostrar(ids[indice]);
                }
            } else if ((nav == 'S' || nav == 's') && pag.haySiguiente()) {
                pag.siguiente();
            } else if ((nav == 'A' || nav == 'a') && pag.hayAnterior()) {
                pag.anterior();
            }
        } while (nav != 'V' && nav != 'v');
    }
};

#endif