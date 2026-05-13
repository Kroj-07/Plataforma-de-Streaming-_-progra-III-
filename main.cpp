// main.cpp — Punto de entrada (Integrante D)
// Jerarquia de inclusion: main.cpp -> Menu.h -> Buscador.h -> Trie.h
#include "src/ui/Menu.h"
#include <iostream>
#include <exception>

int main() {
    try {
        Menu menu;        // Construye Trie, TagIndex, Buscador, UserData, Recommender
        menu.iniciar();   // Bucle while principal con paginacion 5-en-5
    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "[FATAL] error desconocido" << std::endl;
        return 1;
    }
    return 0;
}
