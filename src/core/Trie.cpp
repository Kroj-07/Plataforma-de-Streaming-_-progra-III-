#include "Trie.h"
#include <sstream>
#include <algorithm>
#include <cctype>

struct Trie::NodoTrie {
    std::unordered_map<char, NodoTrie*> hijos;
    std::set<int> peliculasIds;
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

void Trie::insertarPalabra(std::string palabra, int idPelicula) {
    if (palabra.empty()) return;
    for (char& c : palabra) c = std::tolower(static_cast<unsigned char>(c));

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

void Trie::insertarTexto(std::string texto, int idPelicula) {
    for (char& c : texto) c = std::tolower(static_cast<unsigned char>(c));
    std::istringstream stream(texto);
    std::string palabra;
    while (stream >> palabra) {
        while (!palabra.empty() && !std::isalnum(static_cast<unsigned char>(palabra.front()))) palabra.erase(0,1);
        while (!palabra.empty() && !std::isalnum(static_cast<unsigned char>(palabra.back())))  palabra.pop_back();
        if (!palabra.empty()) insertarPalabra(palabra, idPelicula);
    }
}

std::set<int> Trie::buscarSubcadena(std::string subcadena) {
    if (subcadena.empty()) return {};
    for (char& c : subcadena) c = std::tolower(static_cast<unsigned char>(c));
    NodoTrie* actual = raiz;
    for (char c : subcadena) {
        if (!actual->hijos.count(c)) return {};
        actual = actual->hijos[c];
    }
    return actual->peliculasIds;
}

std::set<int> Trie::buscarFrase(std::string frase) {
    std::set<int> resultado;
    for (char& c : frase) c = std::tolower(static_cast<unsigned char>(c));
    std::istringstream stream(frase);
    std::string palabra;
    while (stream >> palabra) {
        std::set<int> parcial = buscarSubcadena(palabra);
        resultado.insert(parcial.begin(), parcial.end());
    }
    return resultado;
}
