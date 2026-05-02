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
    
    // Insertar una palabra (título o sinopsis) asociada a un ID de película
    // Inserta TODOS los sufijos para permitir búsqueda por subcadena
    void insertarPalabra(string palabra, int idPelicula);
    
    // Insertar todas las palabras de un texto (título o sinopsis completa)
    void insertarTexto(string texto, int idPelicula);
    
    // Buscar una subcadena en el Trie
    // Retorna set con los IDs de películas que contienen esa subcadena
    set<int> buscarSubcadena(string subcadena);
    
    // Buscar una frase completa (varias palabras)
    // Retorna la UNION de resultados de cada palabra
    set<int> buscarFrase(string frase);
    
private:
    // Nodo raíz del Trie (implementación interna)
    struct NodoTrie;
    NodoTrie* raiz;
};

#endif