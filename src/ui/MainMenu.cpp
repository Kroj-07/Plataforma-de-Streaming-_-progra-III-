#include "MainMenu.h"
#include "SearchMenu.h"
#include "TagSearchMenu.h"
#include "DetailMenu.h"
#include "WatchLaterMenu.h"
#include "RecommendMenu.h"
#include "LikesMenu.h"
#include "../search/TFIDFScoringStrategy.h"
#include <memory>
#include "../data/CSVReader.h"
#include "../core/Normalizador.h"
#include "TFIDFScoringStrategy.h" 
#include "../search/RelevanceScorer.h"

#include <iostream>
#include <chrono>

using std::cout;
using std::cin;
using std::endl;
using std::string;

MainMenu::MainMenu(Trie& t, TagIndex& tx, Repository<Pelicula>& repo,
                   Buscador& bus, UserData& ud, Recommender& rec)
    : MenuBase(t, tx, repo, bus, ud, rec) {

    // --- NUEVO: Registrar este objeto como observer de UserData ---
    userData.attach(this);  // <-- MainMenu se suscribe a los cambios de UserData

    cout << "Cargando datos (parseo + normalizacion en paralelo)..." << endl;
    cout.flush();

    auto t0 = std::chrono::high_resolution_clock::now();
    std::vector<Pelicula> vec = CSVReader::cargarDatosParalelo("data/processed/peliculas_limpias.csv", tagIndex);
    auto t1 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    cout << "CSV cargado en " << ms << " ms\n";
    cout.flush();
    
    if (vec.empty()) {
        cout << "[MainMenu] CSV vacio o no encontrado. Usando datos simulados.\n";
        vec = cargarDatosSimulados();
    }

    for (const auto& p : vec) {
        repositorio.add(p);
    }

    // --- Asignar estrategia TF-IDF ---
    RelevanceScorer::setEstrategia(std::make_unique<TFIDFScoringStrategy>(repositorio));
    std::cout << "[MainMenu] Estrategia de ranking: TF-IDF Ponderada\n";
    
    cout << "[MainMenu] Indexando " << vec.size() << " peliculas en el Trie...\n";
    cout.flush();

    size_t i = 0;
    for (const auto& p : vec) {
        trie.insertarTexto(p.titulo,   p.id);
        trie.insertarTexto(p.director, p.id);
        trie.insertarTexto(p.casting,  p.id);
        trie.insertarTexto(p.genero,   p.id);
        trie.insertarTexto(p.sinopsis.substr(0, 120), p.id);
        if (++i % 2000 == 0) {
            cout << "  " << i << " / " << vec.size() << "\n";
            cout.flush();
        }
    }
    cout << "[MainMenu] " << vec.size() << " peliculas cargadas.\n";

   
    buscador.setEstrategia(std::make_unique<TFIDFScoringStrategy>(repositorio));
}

// --- NUEVO: Implementación de onDataChanged() ---
void MainMenu::onDataChanged() {
    // Cuando UserData cambia (like o ver más tarde), marcamos la pantalla
    // como desactualizada. En el siguiente ciclo del bucle, se recargará sola.
    pantallaDesactualizada = true;
    // No hacemos cout aquí para no ensuciar la consola.
}

void MainMenu::iniciar() {
    string opcion;
    do {
        // --- NUEVO: Si la pantalla está desactualizada, la recargamos ---
        if (pantallaDesactualizada) {
            pantallaDesactualizada = false;  // Resetear el flag
            // No hacemos limpiarPantalla() aquí porque mostrarPantallaInicio() ya lo hace.
            // Solo forzamos que se vuelva a mostrar.
        }
        
        mostrarPantallaInicio();
        std::getline(cin, opcion);
        ejecutarOpcion(opcion);
    } while (opcion != "0");

    userData.guardar();
    cout << "\nDatos guardados. Hasta luego.\n";
}

void MainMenu::mostrarPantallaInicio() {
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
        std::vector<Pelicula> todas = repositorio.getAll();
        auto recs = recommender.generarRecomendaciones(userData.getLikes(), todas, 3);
        if (recs.empty()) {
            cout << "  Aun no hay recomendaciones\n";
        } else {
            int i = 1;
            for (int id : recs) mostrarFilaPelicula(i++, id);
        }
    }

    cout << "\n------------------------------------------\n";
    cout << "  [1] Buscar pelicula (texto)\n";
    cout << "  [2] Buscar por tag (director/genero/actor)\n";
    cout << "  [3] Ver mas tarde\n";
    cout << "  [4] Ver recomendaciones\n";
    cout << "  [5] Mis likes\n";
    cout << "  [0] Salir\n";
    cout << "------------------------------------------\n";
    cout << "Seleccione una opcion: ";
}

void MainMenu::ejecutarOpcion(const std::string& opcion) {
    if (opcion == "1") {
        SearchMenu searchMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
        searchMenu.mostrar();
    } else if (opcion == "2") {
        TagSearchMenu tagSearchMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
        tagSearchMenu.mostrar();
    } else if (opcion == "3") {
        WatchLaterMenu watchLaterMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
        watchLaterMenu.mostrar();
    } else if (opcion == "4") {
        RecommendMenu recommendMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
        recommendMenu.mostrar();
    } else if (opcion == "5") {
        LikesMenu likesMenu(trie, tagIndex, repositorio, buscador, userData, recommender);
        likesMenu.mostrar();
    }
}

// --- Datos simulados (sin cambios) ---
std::vector<Pelicula> MainMenu::cargarDatosSimulados() {
    std::vector<Pelicula> vec;

    auto add = [&](int anio, const string& titulo, const string& director,
                   const string& casting, const string& genero, const string& sinopsis) {
        Pelicula p;
        p.id       = (int)vec.size();
        p.anio     = anio;
        p.titulo   = Normalizador::normalizar(titulo);
        p.director = Normalizador::normalizar(director);
        p.casting  = Normalizador::normalizar(casting);
        p.genero   = Normalizador::normalizar(genero);
        p.sinopsis = Normalizador::normalizar(sinopsis);
        tagIndex.agregarPelicula(p);
        vec.push_back(p);
    };

    add(2008, "Iron Man", "Jon Favreau", "Robert Downey Jr.",
        "accion, ciencia ficcion", "Un multimillonario construye un traje blindado.");
    add(2010, "Iron Man 2", "Jon Favreau", "Robert Downey Jr.",
        "accion, ciencia ficcion", "Tony Stark enfrenta a Whiplash.");
    add(2013, "Iron Man 3", "Shane Black", "Robert Downey Jr.",
        "accion, ciencia ficcion", "Tony Stark enfrenta al Mandarin.");
    add(1997, "Titanic", "James Cameron", "Leonardo DiCaprio, Kate Winslet",
        "drama, romance", "Historia de amor en el barco que se hunde.");
    add(2009, "Avatar", "James Cameron", "Sam Worthington",
        "accion, ciencia ficcion", "Un marine viaja a Pandora.");
    add(2015, "El Principito", "Mark Osborne", "Jeff Bridges",
        "animacion, fantasia", "Un piloto conoce a un pequeno principe.");
    add(2005, "Batman Inicia", "Christopher Nolan", "Christian Bale",
        "accion, drama", "Bruce Wayne se convierte en Batman.");
    add(2008, "Batman: El Caballero de la Noche", "Christopher Nolan", "Christian Bale, Heath Ledger",
        "accion, drama", "Batman enfrenta al Joker.");
    add(2014, "Interestelar", "Christopher Nolan", "Matthew McConaughey",
        "ciencia ficcion, drama", "Astronautas buscan un nuevo hogar.");
    add(2010, "El Origen", "Christopher Nolan", "Leonardo DiCaprio",
        "accion, ciencia ficcion", "Ladrones de suenos implantan una idea.");
    add(1999, "Matrix", "Hermanas Wachowski", "Keanu Reeves",
        "accion, ciencia ficcion", "Un hacker descubre que la realidad es una simulacion.");
    add(1985, "Volver al Futuro", "Robert Zemeckis", "Michael J. Fox",
        "aventura, ciencia ficcion", "Un joven viaja en el tiempo.");

    return vec;
}