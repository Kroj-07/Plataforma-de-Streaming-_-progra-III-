#include "CSVReader.h"
#include <algorithm>

string normalizar(string texto) {
    transform(texto.begin(), texto.end(), texto.begin(), ::tolower);
    // Aquí podrías añadir lógica para quitar tildes si lo deseas
    return texto;
}

string CSVReader::leerCeldaCSV(stringstream& ss) {
    string celda;
    char c;

    if (!(ss >> c)) return "";

    if (c == '"') {
        // Caso A: La celda está entre comillas (contiene comas o saltos)
        bool escapado = false;
        while (ss.get(c)) {
            if (c == '"') {
                if (ss.peek() == '"') { // Manejo de comillas dobles ""
                    ss.get(c);
                    celda += '"';
                } else {
                    escapado = true;
                    break;
                }
            } else {
                celda += c;
            }
        }
        // Consumir la coma después de la comilla de cierre
        if (ss.peek() == ',') ss.get(c);
    } else {
        // Caso B: Celda simple sin comillas
        ss.putback(c);
        getline(ss, celda, ',');
    }
    return celda;
}

vector<Pelicula> CSVReader::cargarDatos(string rutaArchivo, TagIndex& indexer) {
    vector<Pelicula> peliculas;
    ifstream archivo(rutaArchivo);
    string linea;

    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << rutaArchivo << endl;
        return peliculas;
    }

    // Omitir la primera línea (encabezados)
    getline(archivo, linea);

    int idActual = 0;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        Pelicula p;

        try {
            p.id = idActual++;
            p.anio = stoi(leerCeldaCSV(ss));      // Col A: Release Year
            p.titulo = normalizar(leerCeldaCSV(ss)); // Col B: Title
            leerCeldaCSV(ss);                     // Col C: Origin (Ignorar)
            p.director = normalizar(leerCeldaCSV(ss)); // Col D: Director
            p.casting = normalizar(leerCeldaCSV(ss));  // Col E: Cast
            p.genero = normalizar(leerCeldaCSV(ss));   // Col F: Genre
            leerCeldaCSV(ss);                     // Col G: Wiki Page (Ignorar)
            p.sinopsis = leerCeldaCSV(ss);        // Col H: Plot

            // Indexar tags mientras cargamos (Eficiencia)
            indexer.agregarPelicula(p);

            peliculas.push_back(p);
        } catch (...) {

            continue;
        }
    }

    archivo.close();
    return peliculas;
}