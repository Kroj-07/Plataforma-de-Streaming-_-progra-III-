#include "src/ui/MainMenu.h"

int main() {
    Trie trie;
    TagIndex tagIndex;
    Repository<Pelicula> repositorio;
    Buscador buscador(trie, tagIndex, repositorio);
    UserData& userData = UserData::getInstance();
    Recommender recommender;

    MainMenu menu(trie, tagIndex, repositorio, buscador, userData, recommender);
    menu.iniciar();
    return 0;
}