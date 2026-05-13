#include "../src/core/Trie.h"   // Ajusta la ruta si tu compilador lo requiere
#include <iostream>
#include <set>
#include <string>
#include <cassert>

using namespace std;

static int pasadas = 0;
static int fallidas = 0;

// Imprime el resultado de una prueba y actualiza los contadores.
void reportar(const string& nombre, bool condicion) {
    if (condicion) {
        cout << "  [PASS] " << nombre << "\n";
        pasadas++;
    } else {
        cout << "  [FAIL] " << nombre << "  <-- REVISAR\n";
        fallidas++;
    }
}

void test_insertar_y_buscar_exacto() {
    cout << "\n[1] insertarPalabra + buscarSubcadena — coincidencia exacta\n";

    Trie t;
    t.insertarPalabra("batman", 1);
    t.insertarPalabra("matrix", 2);
    t.insertarPalabra("titanic", 3);

    set<int> res = t.buscarSubcadena("batman");
    reportar("Buscar 'batman' devuelve {1}",
             res == set<int>{1});

    res = t.buscarSubcadena("matrix");
    reportar("Buscar 'matrix' devuelve {2}",
             res == set<int>{2});

    res = t.buscarSubcadena("titanic");
    reportar("Buscar 'titanic' devuelve {3}",
             res == set<int>{3});
}

void test_buscar_por_prefijo() {
    cout << "\n[2] buscarSubcadena — búsqueda por prefijo\n";

    Trie t;
    t.insertarPalabra("batman", 1);
    t.insertarPalabra("bateria", 2);
    t.insertarPalabra("futbol", 3);

    set<int> res = t.buscarSubcadena("bat");
    reportar("Buscar 'bat' devuelve {1,2}",
             res == set<int>{1, 2});

    res = t.buscarSubcadena("fut");
    reportar("Buscar 'fut' devuelve {3}",
             res == set<int>{3});
}

void test_buscar_subcadena_interna() {
    cout << "\n[3] buscarSubcadena — subcadena interna\n";

    Trie t;
    t.insertarPalabra("aironman", 1);
    t.insertarPalabra("ironico",  2);
    t.insertarPalabra("baron",    3);

    set<int> res = t.buscarSubcadena("iron");

    reportar("Buscar 'iron' en {'aironman','ironico','baron'} devuelve {1,2}",
             res.count(1) && res.count(2) && !res.count(3));

    res = t.buscarSubcadena("ron");

    reportar("Buscar 'ron' está en los tres",
             res.count(1) && res.count(2) && res.count(3));
}

void test_buscar_sin_resultado() {
    cout << "\n[4] buscarSubcadena — sin coincidencias\n";

    Trie t;
    t.insertarPalabra("avatar", 10);
    t.insertarPalabra("interestelar", 11);

    set<int> res = t.buscarSubcadena("joker");
    reportar("Buscar 'joker' devuelve conjunto vacío",
             res.empty());

    res = t.buscarSubcadena("xyz");
    reportar("Buscar 'xyz' devuelve conjunto vacío",
             res.empty());
}

void test_misma_pelicula_varias_palabras() {
    cout << "\n[5] insertarPalabra — varias palabras, mismo ID\n";

    Trie t;
    t.insertarPalabra("iron",  7);
    t.insertarPalabra("man",   7);
    t.insertarPalabra("stark", 7);

    set<int> resIron  = t.buscarSubcadena("iron");
    set<int> resMan   = t.buscarSubcadena("man");
    set<int> resStark = t.buscarSubcadena("stark");

    reportar("'iron'  lleva al ID 7", resIron.count(7));
    reportar("'man'   lleva al ID 7", resMan.count(7));
    reportar("'stark' lleva al ID 7", resStark.count(7));
}

void test_misma_palabra_varias_peliculas() {
    cout << "\n[6] insertarPalabra — misma palabra, distintos IDs\n";

    Trie t;
    t.insertarPalabra("nolan", 8);   // Batman
    t.insertarPalabra("nolan", 9);   // Interestelar
    t.insertarPalabra("nolan", 10);  // El Origen

    set<int> res = t.buscarSubcadena("nolan");
    reportar("'nolan' devuelve {8,9,10}",
             res == set<int>{8, 9, 10});
}

// 7. insertarTexto — texto completo
void test_insertar_texto_completo() {
    cout << "\n[7] insertarTexto — texto con varias palabras\n";

    Trie t;
    t.insertarTexto("un piloto conoce a un principe en el desierto", 6);

    set<int> resPiloto   = t.buscarSubcadena("piloto");
    set<int> resPrincipe = t.buscarSubcadena("principe");
    set<int> resDesierto = t.buscarSubcadena("desierto");

    reportar("'piloto' encontrado en película 6",   resPiloto.count(6));
    reportar("'principe' encontrado en película 6", resPrincipe.count(6));
    reportar("'desierto' encontrado en película 6", resDesierto.count(6));
}

void test_insertar_texto_subcadena() {
    cout << "\n[8] insertarTexto — subcadena de palabra del texto\n";

    Trie t;
    t.insertarTexto("ladrones de suenos en capas", 10); // El Origen

    set<int> res = t.buscarSubcadena("sue");   // parte de "suenos"
    reportar("'sue' es subcadena de 'suenos' → encuentra película 10",
             res.count(10));

    res = t.buscarSubcadena("cap");           // parte de "capas"
    reportar("'cap' es subcadena de 'capas' → encuentra película 10",
             res.count(10));
}

void test_buscar_frase() {
    cout << "\n[9] buscarFrase — unión de resultados por palabra\n";

    Trie t;
    t.insertarTexto("astronautas viajan al espacio", 9);   // Interestelar
    t.insertarTexto("viajan en barco por el oceano",  4);  // Titanic
    t.insertarTexto("el espacio es infinito",          5); // Avatar

    set<int> res = t.buscarFrase("viajan espacio");
    reportar("Frase 'viajan espacio' incluye IDs 4,5,9",
             res.count(4) && res.count(5) && res.count(9));
}

void test_buscar_frase_palabra_inexistente() {
    cout << "\n[10] buscarFrase — una palabra no existe\n";

    Trie t;
    t.insertarTexto("guerreros del pasado", 20);

    set<int> res = t.buscarFrase("guerreros joker");
    reportar("'guerreros joker' encuentra película 20 (por 'guerreros')",
             res.count(20));

    res = t.buscarFrase("naves extraterrestres xyz");
    reportar("Frase totalmente inexistente → conjunto vacío",
             res.empty());
}

void test_trie_vacio() {
    cout << "\n[11] Trie vacío — búsquedas en Trie sin inserción\n";

    Trie t;
    reportar("buscarSubcadena en Trie vacío devuelve vacío",
             t.buscarSubcadena("batman").empty());
    reportar("buscarFrase en Trie vacío devuelve vacío",
             t.buscarFrase("iron man").empty());
}

void test_cadena_vacia() {
    cout << "\n[12] Cadena vacía como argumento\n";

    Trie t;
    t.insertarPalabra("avatar", 1);

    bool noExplota = true;
    try {
        t.buscarSubcadena("");
    } catch (...) {
        noExplota = false;
    }
    reportar("buscarSubcadena('') no lanza excepción", noExplota);
}

void resumen() {
    cout << "\n══════════════════════════════════════════\n";
    cout << "  RESUMEN DE PRUEBAS\n";
    cout << "──────────────────────────────────────────\n";
    cout << "  Pasadas : " << pasadas  << "\n";
    cout << "  Fallidas: " << fallidas << "\n";
    if (fallidas == 0) {
        cout << "  ✔ Todos los tests pasaron correctamente.\n";
    } else {
        cout << "  ✘ Hay " << fallidas << " test(s) fallando. Revisar implementación.\n";
    }
    cout << "══════════════════════════════════════════\n";
}

// ── main ───────────────────────────────────────────────────

int main() {
    cout << "╔══════════════════════════════════════════╗\n";
    cout << "║   TEST SUITE — Trie (Plataforma Stream)  ║\n";
    cout << "╚══════════════════════════════════════════╝\n";

    test_insertar_y_buscar_exacto();
    test_buscar_por_prefijo();
    test_buscar_subcadena_interna();
    test_buscar_sin_resultado();
    test_misma_pelicula_varias_palabras();
    test_misma_palabra_varias_peliculas();
    test_insertar_texto_completo();
    test_insertar_texto_subcadena();
    test_buscar_frase();
    test_buscar_frase_palabra_inexistente();
    test_trie_vacio();
    test_cadena_vacia();

    resumen();
    return fallidas == 0 ? 0 : 1;
}