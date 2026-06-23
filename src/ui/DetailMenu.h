#ifndef DETAILMENU_H
#define DETAILMENU_H

#include "MenuBase.h"
#include <iostream>
#include <string>

class DetailMenu : public MenuBase {
public:
    DetailMenu(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
               Buscador& bus, UserData& ud, Recommender& rec)
        : MenuBase(t, tx, repo, bus, ud, rec) {}

    void mostrar(int id) {
        if (!repositorio.exists(id)) {
            std::cout << "Pelicula no encontrada.\n";
            pausar();
            return;
        }

        const Pelicula& p = repositorio.getById(id);

        limpiarPantalla();
        std::cout << "==========================================\n";
        std::cout << "  " << p.titulo << " (" << p.anio << ")\n";
        std::cout << "==========================================\n\n";
        std::cout << "Director: " << p.director << "\n";
        std::cout << "Genero:   " << p.genero   << "\n";
        std::cout << "Casting:  " << p.casting  << "\n\n";
        std::cout << "Sinopsis:\n" << p.sinopsis << "\n\n";

        bool tieneLike = userData.tieneLike(id);
        bool enVer     = userData.estaEnVerMasTarde(id);
        std::cout << "Estado:";
        if (tieneLike) std::cout << " [LIKE]";
        if (enVer)     std::cout << " [VER MAS TARDE]";
        if (!tieneLike && !enVer) std::cout << " (sin marcas)";
        std::cout << "\n\n";

        std::cout << "==========================================\n";
        std::cout << "  [L] Like  [V] Ver mas tarde  [Q] Quitar marcas  [Enter] Volver\n";
        std::cout << "Seleccione: ";

        std::string opcion;
        std::getline(std::cin, opcion);
        if (!opcion.empty()) {
            procesarAccion(id, opcion[0]);
        }
    }

private:
    void procesarAccion(int id, char accion) {
        bool tieneLike = userData.tieneLike(id);
        bool enVer     = userData.estaEnVerMasTarde(id);

        if (accion == 'L' || accion == 'l') {
            if (tieneLike) std::cout << ">> Ya le diste like a esta pelicula.\n";
            else { userData.darLike(id); std::cout << ">> Like agregado.\n"; }
        } else if (accion == 'V' || accion == 'v') {
            if (enVer) std::cout << ">> Ya esta en 'Ver mas tarde'.\n";
            else { userData.agregarVerMasTarde(id); std::cout << ">> Agregado a 'Ver mas tarde'.\n"; }
        } else if (accion == 'Q' || accion == 'q') {
            userData.quitarLike(id);
            userData.quitarVerMasTarde(id);
            std::cout << ">> Marcas removidas.\n";
        }
        pausar();
    }
};

#endif