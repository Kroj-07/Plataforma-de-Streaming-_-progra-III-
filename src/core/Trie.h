#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <set>
#include <unordered_map>
#include <vector>

// Suffix Trie — implementación de A.
// Almacena todos los sufijos de cada palabra para permitir búsqueda
// por subcadena en O(k) donde k es la longitud del query.
class Trie {
public:
    Trie();
    ~Trie();

    // Regla de 5: el Trie posee memoria cruda (nodos con new/delete). Copiar
    // el objeto haria una copia superficial de los punteros -> doble free al
    // destruir ambas copias. Como no necesitamos copiarlo (se pasa siempre por
    // referencia), deshabilitamos copia y movimiento explicitamente.
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    Trie(Trie&&) = delete;
    Trie& operator=(Trie&&) = delete;

    void insertarPalabra(std::string palabra, int idPelicula);

    void insertarTexto(std::string texto, int idPelicula);

    std::set<int> buscarSubcadena(std::string subcadena);

    std::set<int> buscarFrase(std::string frase);
    
private:
    struct NodoTrie;
    NodoTrie* raiz;

    void destruirNodos(NodoTrie* nodo);
};

#endif
