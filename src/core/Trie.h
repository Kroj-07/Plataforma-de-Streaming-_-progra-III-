#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <set>
#include <vector>
using namespace std;

class Trie {
public:
    Trie();
    ~Trie();

    void insertarPalabra(string palabra, int idPelicula);

    void insertarTexto(string texto, int idPelicula);

    set<int> buscarSubcadena(string subcadena);

    set<int> buscarFrase(string frase);
    
private:
    struct NodoTrie;
    NodoTrie* raiz;
};

#endif