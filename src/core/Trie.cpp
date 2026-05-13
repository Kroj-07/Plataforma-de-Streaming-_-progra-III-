#include "Trie.h"
#include <sstream>
#include <algorithm>

struct Trie::NodoTrie {
    map<char, NodoTrie*> hijos;
    set<int> peliculasIds;
    NodoTrie() {}
};

Trie::Trie()  { raiz = new NodoTrie(); }
Trie::~Trie() { destruirNodos(raiz);   }

void Trie::destruirNodos(NodoTrie* nodo) {
    if (!nodo) return;
    for (auto& par : nodo->hijos)
        destruirNodos(par.second);
    delete nodo;
}

void Trie::insertarPalabra(string palabra, int idPelicula) {
    if (palabra.empty()) return;
    for (char& c : palabra) c = tolower(c);

    for (int inicio = 0; inicio < (int)palabra.size(); inicio++) {
        NodoTrie* actual = raiz;
        for (int i = inicio; i < (int)palabra.size(); i++) {
            char c = palabra[i];
            if (!actual->hijos.count(c))
                actual->hijos[c] = new NodoTrie();
            actual = actual->hijos[c];
            actual->peliculasIds.insert(idPelicula);
        }
    }
}

void Trie::insertarTexto(string texto, int idPelicula) {
    for (char& c : texto) c = tolower(c);
    istringstream stream(texto);
    string palabra;
    while (stream >> palabra) {
        while (!palabra.empty() && !isalnum((unsigned char)palabra.front())) palabra.erase(0,1);
        while (!palabra.empty() && !isalnum((unsigned char)palabra.back()))  palabra.pop_back();
        if (!palabra.empty()) insertarPalabra(palabra, idPelicula);
    }
}

set<int> Trie::buscarSubcadena(string subcadena) {
    if (subcadena.empty()) return {};
    for (char& c : subcadena) c = tolower(c);
    NodoTrie* actual = raiz;
    for (char c : subcadena) {
        if (!actual->hijos.count(c)) return {};
        actual = actual->hijos[c];
    }
    return actual->peliculasIds;
}

set<int> Trie::buscarFrase(string frase) {
    set<int> resultado;
    for (char& c : frase) c = tolower(c);
    istringstream stream(frase);
    string palabra;
    while (stream >> palabra) {
        set<int> parcial = buscarSubcadena(palabra);
        resultado.insert(parcial.begin(), parcial.end());
    }
    return resultado;
}
