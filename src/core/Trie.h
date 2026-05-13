#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <set>
#include <map>
#include <vector>

// Suffix Trie — implementación de A.
// Almacena todos los sufijos de cada palabra para permitir búsqueda
// por subcadena en O(k) donde k es la longitud del query.
class Trie {
public:
    Trie();
    ~Trie();

    // Inserta una palabra y todos sus sufijos.  Costo: O(m^2).
    void insertarPalabra(const std::string& palabra, int idPelicula);

    // Tokeniza el texto y llama insertarPalabra por cada token.
    void insertarTexto(const std::string& texto, int idPelicula);

    // Devuelve el conjunto de IDs cuyos textos contienen la subcadena.
    std::set<int> buscarSubcadena(const std::string& subcadena);

    // Devuelve la UNIÓN de buscarSubcadena por cada palabra de la frase.
    std::set<int> buscarFrase(const std::string& frase);

private:
    struct NodoTrie {
        std::map<char, NodoTrie*> hijos;
        std::set<int> peliculasIds;
    };

    NodoTrie* raiz;

    void destruirNodos(NodoTrie* nodo);
};

#endif
