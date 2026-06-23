#ifndef MENUBASE_H
#define MENUBASE_H

#include "../core/Trie.h"
#include "../core/Repository.h"
#include "../data/TagIndex.h"
#include "../data/Pelicula.h"
#include "../search/Buscador.h"
#include "../user/UserData.h"
#include "../user/Recommender.h"
#include <iostream>
#include <string>

// Clase base para todos los menús. Contiene referencias a los objetos compartidos
// y métodos de utilidad comunes.
class MenuBase {
    
protected:
    // Referencias a los objetos compartidos (se pasan por referencia en el constructor)
    Trie& trie;
    TagIndex& tagIndex;
    Repository<Pelicula>& repositorio;
    Buscador& buscador;
    UserData& userData;
    Recommender& recommender;

public:
    MenuBase(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
             Buscador& bus, UserData& ud, Recommender& rec)
        : trie(t), tagIndex(tx), repositorio(repo),
          buscador(bus), userData(ud), recommender(rec) {}

    // Métodos de utilidad (comunes a todos los menús)
    void limpiarPantalla() const {
        std::cout << "\n\n==========================================\n\n";
    }

    void pausar() const {
        std::cout << "\nPresione Enter para continuar...";
        std::cin.get();
        std::cin.get();
    }

    void mostrarFilaPelicula(int indiceDisplay, int idPelicula) const {
        if (!repositorio.exists(idPelicula)) return;
        const Pelicula& p = repositorio.getById(idPelicula);
        std::cout << "  [" << indiceDisplay << "] " << p.titulo << " (" << p.anio << ")\n";
        std::cout << "      Director: " << p.director << "\n\n";
    }

    // Destructor virtual por si se necesita en el futuro
    virtual ~MenuBase() = default;
};

#endif