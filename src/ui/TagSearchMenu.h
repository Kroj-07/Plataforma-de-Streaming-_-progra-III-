#ifndef TAGSEARCHMENU_H
#define TAGSEARCHMENU_H

#include "MenuBase.h"
#include "Paginador.h"
#include "DetailMenu.h"
#include <iostream>
#include <string>
#include <vector>

// Busqueda por TAG (director / genero / casting) usando el TagIndex (O(1)).
// Cumple el requisito del enunciado: "buscar peliculas por un Tag".
class TagSearchMenu : public MenuBase {
public:
    TagSearchMenu(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
                  Buscador& bus, UserData& ud, Recommender& rec)
        : MenuBase(t, tx, repo, bus, ud, rec) {}

    void mostrar() {
        limpiarPantalla();
        std::cout << "==========================================\n";
        std::cout << "  BUSCAR POR TAG\n";
        std::cout << "==========================================\n";
        std::cout << "  [1] Director\n";
        std::cout << "  [2] Genero\n";
        std::cout << "  [3] Actor (casting)\n";
        std::cout << "  [V] Volver\n";
        std::cout << "Seleccione el tipo de tag: ";

        std::string opcion;
        std::getline(std::cin, opcion);
        if (opcion.empty() || opcion[0] == 'V' || opcion[0] == 'v') return;

        std::string tipo;
        if      (opcion[0] == '1') tipo = "director";
        else if (opcion[0] == '2') tipo = "genero";
        else if (opcion[0] == '3') tipo = "casting";
        else return;

        std::cout << "\nIngrese el " << tipo << " a buscar: ";
        std::string valor;
        std::getline(std::cin, valor);
        if (valor.empty()) return;

        std::set<int> ids = buscador.buscarTag(tipo, valor);
        if (ids.empty()) {
            std::cout << "\nNo se encontraron peliculas para " << tipo
                      << " = \"" << valor << "\"\n";
            pausar();
            return;
        }

        std::vector<int> resultados(ids.begin(), ids.end());
        mostrarResultados(tipo + " = " + valor, resultados);
    }

private:
    void mostrarResultados(const std::string& titulo, const std::vector<int>& ids) {
        Paginador pag((int)ids.size(), 5);
        char nav = 0;

        do {
            limpiarPantalla();
            std::cout << "==========================================\n";
            std::cout << "  Resultados: " << titulo << "\n";
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
