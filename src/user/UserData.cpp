#include "UserData.h"
#include <fstream>
#include <iostream>
#include <stdexcept>



const string UserData::ARCHIVO_LIKES          = "user_data/likes.txt";
const string UserData::ARCHIVO_VER_MAS_TARDE  = "user_data/ver_mas_tarde.txt";

UserData::UserData() {
    cargar();
}

void UserData::darLike(int idPelicula) {
    likes.insert(idPelicula);
}

void UserData::quitarLike(int idPelicula) {
    likes.erase(idPelicula);
}

bool UserData::tieneLike(int idPelicula) const {
    return likes.count(idPelicula) > 0;
}

const set<int>& UserData::getLikes() const {
    return likes;
}
void UserData::agregarVerMasTarde(int idPelicula) {
    verMasTarde.insert(idPelicula);
}

void UserData::quitarVerMasTarde(int idPelicula) {
    verMasTarde.erase(idPelicula);
}

bool UserData::estaEnVerMasTarde(int idPelicula) const {
    return verMasTarde.count(idPelicula) > 0;
}

const set<int>& UserData::getVerMasTarde() const {
    return verMasTarde;
}
void UserData::guardarArchivo(const set<int>& datos, const string& ruta) const {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "[UserData] No se pudo abrir para escritura: " << ruta << "\n";
        return;
    }
    for (int id : datos) {
        archivo << id << "\n";
    }
}

void UserData::cargarArchivo(set<int>& datos, const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        return;
    }
    int id;
    while (archivo >> id) {
        datos.insert(id);
    }
}

void UserData::guardar() const {
    guardarArchivo(likes,        ARCHIVO_LIKES);
    guardarArchivo(verMasTarde,  ARCHIVO_VER_MAS_TARDE);
}

void UserData::cargar() {
    cargarArchivo(likes,        ARCHIVO_LIKES);
    cargarArchivo(verMasTarde,  ARCHIVO_VER_MAS_TARDE);
}
