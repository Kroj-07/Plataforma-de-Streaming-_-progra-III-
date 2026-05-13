#include "Trie.h"
#include <sstream>
#include <algorithm>
#include <cctype>

using std::string;
using std::set;
using std::istringstream;

Trie::Trie()  { raiz = new NodoTrie(); }
Trie::~Trie() { destruirNodos(raiz);    }

void Trie::destruirNodos(NodoTrie* nodo) {
    if (!nodo) return;
    for (auto& par : nodo->hijos) destruirNodos(par.second);
    delete nodo;
}

void Trie::insertarPalabra(const string& palabraIn, int idPelicula) {
    if (palabraIn.empty()) return;
    string palabra = palabraIn;
    for (char& c : palabra) c = static_cast<char>(std::tolower((unsigned char)c));

    for (int inicio = 0; inicio < (int)palabra.size(); ++inicio) {
        NodoTrie* actual = raiz;
        for (int i = inicio; i < (int)palabra.size(); ++i) {
            char c = palabra[i];
            auto it = actual->hijos.find(c);
            if (it == actual->hijos.end()) {
                actual = actual->hijos[c] = new NodoTrie();
            } else {
                actual = it->second;
            }
            actual->peliculasIds.insert(idPelicula);
        }
    }
}

void Trie::insertarTexto(const string& textoIn, int idPelicula) {
    string texto = textoIn;
    for (char& c : texto) c = static_cast<char>(std::tolower((unsigned char)c));

    istringstream stream(texto);
    string palabra;
    while (stream >> palabra) {
        while (!palabra.empty() && !std::isalnum((unsigned char)palabra.front())) palabra.erase(0, 1);
        while (!palabra.empty() && !std::isalnum((unsigned char)palabra.back()))  palabra.pop_back();
        if (!palabra.empty()) insertarPalabra(palabra, idPelicula);
    }
}

set<int> Trie::buscarSubcadena(const string& subcadenaIn) {
    if (subcadenaIn.empty()) return {};
    string subcadena = subcadenaIn;
    for (char& c : subcadena) c = static_cast<char>(std::tolower((unsigned char)c));

    NodoTrie* actual = raiz;
    for (char c : subcadena) {
        auto it = actual->hijos.find(c);
        if (it == actual->hijos.end()) return {};
        actual = it->second;
    }
    return actual->peliculasIds;
}

set<int> Trie::buscarFrase(const string& fraseIn) {
    set<int> resultado;
    string frase = fraseIn;
    for (char& c : frase) c = static_cast<char>(std::tolower((unsigned char)c));

    istringstream stream(frase);
    string palabra;
    while (stream >> palabra) {
        set<int> parcial = buscarSubcadena(palabra);
        resultado.insert(parcial.begin(), parcial.end());
    }
    return resultado;
}
