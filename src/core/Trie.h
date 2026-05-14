#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <set>
#include <map>
#include <vector>

using namespace std;


// Suffix Trie — implementación de A.
// Almacena todos los sufijos de cada palabra para permitir búsqueda
// por subcadena en O(k) donde k es la longitud del query.
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

    void destruirNodos(NodoTrie* nodo);
};

#endif
