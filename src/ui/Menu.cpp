#include "Menu.h"
#include "Paginador.h"
#include "../core/Normalizador.h"
#include "../search/RelevanceScorer.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::set;

// -------- Construccion ----------------------------------------------------
Menu::Menu()
    : trie(),
      tagIndex(),
      peliculas(),
      buscador(trie, tagIndex, peliculas),
      userData(),
      recommender() {

    // 1. Cargar CSV real si existe; si esta vacio, usar datos simulados.
    peliculas = CSVReader::cargarDatos("data/processed/peliculas_limpias.csv", tagIndex);
    if (peliculas.empty()) {
        cout << "[Menu] CSV vacio o no encontrado. Usando datos simulados.\n";
        cargarDatosSimulados();
    }

    // 2. Construir el Trie a partir de titulo + sinopsis + director + casting + genero.
    //    Asi una busqueda como "nolan" o "leonardo" encuentra peliculas.
    for (const auto& p : peliculas) {
        trie.insertarTexto(p.titulo,   p.id);
        trie.insertarTexto(p.sinopsis, p.id);
        trie.insertarTexto(p.director, p.id);
        trie.insertarTexto(p.casting,  p.id);
        trie.insertarTexto(p.genero,   p.id);
    }

    cout << "[Menu] " << peliculas.size() << " peliculas cargadas.\n";
}

// -------- Datos simulados (Semana 8) --------------------------------------
void Menu::cargarDatosSimulados() {
    auto add = [&](int anio, const string& titulo, const string& director,
                   const string& casting, const string& genero, const string& sinopsis) {
        Pelicula p;
        p.id       = (int)peliculas.size();
        p.anio     = anio;
        p.titulo   = Normalizador::normalizar(titulo);
        p.director = Normalizador::normalizar(director);
        p.casting  = Normalizador::normalizar(casting);
        p.genero   = Normalizador::normalizar(genero);
        p.sinopsis = Normalizador::normalizar(sinopsis);
        tagIndex.agregarPelicula(p);
        peliculas.push_back(p);
    };

    add(2008, "Iron Man", "Jon Favreau", "Robert Downey Jr.",
        "accion, ciencia ficcion",
        "Un multimillonario construye un traje blindado para enfrentar terroristas.");
    add(2010, "Iron Man 2", "Jon Favreau", "Robert Downey Jr.",
        "accion, ciencia ficcion",
        "Tony Stark enfrenta a Whiplash mientras lidia con su legado.");
    add(2013, "Iron Man 3", "Shane Black", "Robert Downey Jr.",
        "accion, ciencia ficcion",
        "Tony Stark enfrenta al Mandarin y debe sobrevivir sin su armadura.");
    add(1997, "Titanic", "James Cameron", "Leonardo DiCaprio, Kate Winslet",
        "drama, romance",
        "Historia de amor a bordo del barco que se hunde en el Atlantico.");
    add(2009, "Avatar", "James Cameron", "Sam Worthington, Zoe Saldana",
        "accion, ciencia ficcion",
        "Un marine viaja a Pandora y se une al pueblo nativo Navi.");
    add(2015, "El Principito", "Mark Osborne", "Jeff Bridges, Mackenzie Foy",
        "animacion, fantasia",
        "Un piloto cuenta la historia de un pequeno principe de otro planeta.");
    add(2005, "Batman Inicia", "Christopher Nolan", "Christian Bale",
        "accion, drama",
        "Bruce Wayne viaja por el mundo y se convierte en Batman.");
    add(2008, "Batman: El Caballero de la Noche", "Christopher Nolan", "Christian Bale, Heath Ledger",
        "accion, drama",
        "Batman se enfrenta al Joker en una guerra por el alma de Gotham.");
    add(2014, "Interestelar", "Christopher Nolan", "Matthew McConaughey, Anne Hathaway",
        "ciencia ficcion, drama",
        "Astronautas viajan a traves de un agujero de gusano buscando un nuevo hogar.");
    add(2010, "El Origen", "Christopher Nolan", "Leonardo DiCaprio",
        "accion, ciencia ficcion",
        "Ladrones de suenos se infiltran en la mente para implantar una idea.");
    add(1999, "Matrix", "Hermanas Wachowski", "Keanu Reeves",
        "accion, ciencia ficcion",
        "Un hacker descubre que la realidad es una simulacion controlada por maquinas.");
    add(1985, "Volver al Futuro", "Robert Zemeckis", "Michael J. Fox",
        "aventura, ciencia ficcion",
        "Un joven viaja en el tiempo en un auto modificado por un cientifico excentrico.");
}

// -------- Utilidades ------------------------------------------------------
void Menu::pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
}

// -------- Pantalla de Inicio ---------------------------------------------
void Menu::pantallaInicio() {
    limpiarPantalla();
    cout << "+------------------------------------------+\n";
    cout << "|   PLATAFORMA DE STREAMING                |\n";
    cout << "|   Proyecto Final - Programacion III      |\n";
    cout << "+------------------------------------------+\n\n";

    cout << "------------------------------------------\n";
    cout << "  VER MAS TARDE\n";
    cout << "------------------------------------------\n";
    if (userData.getVerMasTarde().empty()) {
        cout << "  (vacio) - busca peliculas y guardalas\n";
    } else {
        int i = 1;
        for (int id : userData.getVerMasTarde()) {
            mostrarFilaPelicula(i++, id);
            if (i > 3) break;
        }
    }

    cout << "\n------------------------------------------\n";
    cout << "  RECOMENDACIONES PARA TI\n";
    cout << "------------------------------------------\n";
    if (userData.getLikes().empty()) {
        cout << "  Da like a peliculas para ver recomendaciones\n";
    } else {
        auto recs = recommender.generarRecomendaciones(userData.getLikes(), peliculas, 3);
        if (recs.empty()) {
            cout << "  Aun no hay recomendaciones\n";
        } else {
            int i = 1;
            for (int id : recs) mostrarFilaPelicula(i++, id);
        }
    }

    cout << "\n------------------------------------------\n";
    cout << "  [1] Buscar\n";
    cout << "  [2] Ver mas tarde (completo)\n";
    cout << "  [3] Recomendaciones (completo)\n";
    cout << "  [0] Salir\n";
    cout << "Seleccione: ";
}

// -------- Busqueda --------------------------------------------------------
void Menu::menuBuscar() {
    limpiarPantalla();
    cout << "==========================================\n";
    cout << "  BUSCAR PELICULA\n";
    cout << "==========================================\n";
    cout << "Puede buscar por palabra, frase o parte de una palabra.\n";
    cout << "Ejemplos: 'iron', 'barco', 'bat', 'nolan'\n\n";
    cout << "Ingrese su busqueda: ";

    string consulta;
    std::getline(cin, consulta);
    if (consulta.empty()) return;

    vector<int> resultados = buscador.buscarOrdenado(consulta);

    if (resultados.empty()) {
        cout << "\nNo se encontraron coincidencias para: \"" << consulta << "\"\n";
        pausar();
        return;
    }

    Paginador pag((int)resultados.size(), 5);
    char nav = 0;

    do {
        limpiarPantalla();
        cout << "==========================================\n";
        cout << "  Resultados para: \"" << consulta << "\"\n";
        cout << "==========================================\n\n";

        for (int i = pag.getInicio(); i < pag.getFin(); ++i) {
            mostrarFilaPelicula(i - pag.getInicio() + 1, resultados[i]);
        }

        cout << "------------------------------------------\n";
        cout << "  Pagina " << pag.getPaginaActual() << " de " << pag.getTotalPaginas();
        cout << "  (Mostrando " << (pag.getFin() - pag.getInicio())
             << " de " << pag.getTotalResultados() << ")\n";
        cout << "------------------------------------------\n";
        cout << "  [1-5] Ver detalle  ";
        if (pag.hayAnterior())  cout << "[A] Anterior  ";
        if (pag.haySiguiente()) cout << "[S] Siguiente  ";
        cout << "[V] Volver\n";
        cout << "Seleccione: ";

        string entrada;
        std::getline(cin, entrada);
        if (entrada.empty()) { nav = 0; continue; }
        nav = entrada[0];

        if (nav >= '1' && nav <= '5') {
            int indice = pag.getInicio() + (nav - '1');
            if (indice < pag.getTotalResultados()) {
                mostrarDetallePelicula(resultados[indice]);
            }
        } else if ((nav == 'S' || nav == 's') && pag.haySiguiente()) {
            pag.siguiente();
        } else if ((nav == 'A' || nav == 'a') && pag.hayAnterior()) {
            pag.anterior();
        }
    } while (nav != 'V' && nav != 'v');
}

// -------- Detalle ---------------------------------------------------------
void Menu::mostrarDetallePelicula(int id) {
    if (id < 0 || id >= (int)peliculas.size()) {
        cout << "Pelicula no encontrada.\n";
        pausar();
        return;
    }
    const Pelicula& p = peliculas[id];

    limpiarPantalla();
    cout << "==========================================\n";
    cout << "  " << p.titulo << " (" << p.anio << ")\n";
    cout << "==========================================\n\n";
    cout << "Director: " << p.director << "\n";
    cout << "Genero:   " << p.genero   << "\n";
    cout << "Casting:  " << p.casting  << "\n\n";
    cout << "Sinopsis:\n" << p.sinopsis << "\n\n";

    bool tieneLike = userData.tieneLike(id);
    bool enVer     = userData.estaEnVerMasTarde(id);
    cout << "Estado:";
    if (tieneLike) cout << " [LIKE]";
    if (enVer)     cout << " [VER MAS TARDE]";
    if (!tieneLike && !enVer) cout << " (sin marcas)";
    cout << "\n\n";

    cout << "==========================================\n";
    cout << "  [L] Like  [V] Ver mas tarde  [Q] Quitar marcas  [Enter] Volver\n";
    cout << "Seleccione: ";

    string opcion;
    std::getline(cin, opcion);
    if (opcion.empty()) return;
    char c = opcion[0];

    if (c == 'L' || c == 'l') {
        if (tieneLike) cout << ">> Ya le diste like a esta pelicula.\n";
        else { userData.darLike(id); cout << ">> Like agregado.\n"; }
    } else if (c == 'V' || c == 'v') {
        if (enVer) cout << ">> Ya esta en 'Ver mas tarde'.\n";
        else { userData.agregarVerMasTarde(id); cout << ">> Agregado a 'Ver mas tarde'.\n"; }
    } else if (c == 'Q' || c == 'q') {
        userData.quitarLike(id);
        userData.quitarVerMasTarde(id);
        cout << ">> Marcas removidas.\n";
    }
    pausar();
}

// -------- Ver mas tarde (pantalla completa) ------------------------------
void Menu::menuVerMasTarde() {
    limpiarPantalla();
    cout << "==========================================\n";
    cout << "  VER MAS TARDE\n";
    cout << "==========================================\n\n";

    const auto& vmt = userData.getVerMasTarde();
    if (vmt.empty()) {
        cout << "No tienes peliculas guardadas en 'Ver mas tarde'.\n";
        pausar();
        return;
    }

    vector<int> ids(vmt.begin(), vmt.end());
    for (size_t i = 0; i < ids.size(); ++i) {
        mostrarFilaPelicula((int)i + 1, ids[i]);
    }

    cout << "------------------------------------------\n";
    cout << "Numero para ver detalle, [V] para volver: ";

    string opcion;
    std::getline(cin, opcion);
    if (opcion.empty() || opcion[0] == 'V' || opcion[0] == 'v') return;

    try {
        int indice = std::stoi(opcion);
        if (indice >= 1 && indice <= (int)ids.size()) {
            mostrarDetallePelicula(ids[indice - 1]);
        }
    } catch (...) { /* entrada invalida, volver */ }
}

// -------- Recomendaciones (pantalla completa) -----------------------------
void Menu::menuRecomendaciones() {
    limpiarPantalla();
    cout << "==========================================\n";
    cout << "  RECOMENDACIONES PARA TI\n";
    cout << "  (basado en tus likes)\n";
    cout << "==========================================\n\n";

    if (userData.getLikes().empty()) {
        cout << "Aun no has dado like a ninguna pelicula.\n";
        cout << "Busca peliculas y dales like para recibir recomendaciones.\n";
        pausar();
        return;
    }

    auto recs = recommender.generarRecomendaciones(userData.getLikes(), peliculas, 5);
    if (recs.empty()) {
        cout << "No hay recomendaciones disponibles aun.\n";
        pausar();
        return;
    }

    for (size_t i = 0; i < recs.size(); ++i) {
        mostrarFilaPelicula((int)i + 1, recs[i]);
    }

    cout << "------------------------------------------\n";
    cout << "Numero para ver detalle, [V] para volver: ";

    string opcion;
    std::getline(cin, opcion);
    if (opcion.empty() || opcion[0] == 'V' || opcion[0] == 'v') return;

    try {
        int indice = std::stoi(opcion);
        if (indice >= 1 && indice <= (int)recs.size()) {
            mostrarDetallePelicula(recs[indice - 1]);
        }
    } catch (...) { /* entrada invalida */ }
}

// -------- Bucle principal ------------------------------------------------
void Menu::iniciar() {
    string opcion;
    do {
        pantallaInicio();
        std::getline(cin, opcion);

        if      (opcion == "1") menuBuscar();
        else if (opcion == "2") menuVerMasTarde();
        else if (opcion == "3") menuRecomendaciones();
    } while (opcion != "0");

    userData.guardar();
    cout << "\nDatos guardados. Hasta luego.\n";
}
