#include "UserData.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

using std::cerr;
using std::ofstream;
using std::ifstream;

const std::string UserData::ARCHIVO_LIKES          = "user_data/likes.txt";
const std::string UserData::ARCHIVO_VER_MAS_TARDE  = "user_data/ver_mas_tarde.txt";

UserData::UserData() {
    cargar();
}

// Meyer's Singleton: la instancia estatica local se construye una unica vez,
// de forma perezosa (en el primer llamado) y con destruccion automatica al
// finalizar el programa. Es thread-safe desde C++11.
UserData& UserData::getInstance() {
    static UserData instancia;
    return instancia;
}

// ============================================================
// MÉTODOS OBSERVER
// ============================================================

void UserData::attach(IObserver* observer) {
    observers.push_back(observer);
}

void UserData::detach(IObserver* observer) {
    // Buscar y eliminar el observer
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == observer) {
            observers.erase(it);
            break;
        }
    }
}

void UserData::notify() {
    // Notificar a TODOS los observers registrados
    for (IObserver* obs : observers) {
        obs->onDataChanged();
    }
}

// ============================================================
// MÉTODOS EXISTENTES (MODIFICADOS CON NOTIFY)
// ============================================================

void UserData::darLike(int idPelicula) {
    likes.insert(idPelicula);
    notify();  // <-- NUEVO: notificar cambio
}

void UserData::quitarLike(int idPelicula) {
    likes.erase(idPelicula);
    notify();  // <-- NUEVO: notificar cambio
}

bool UserData::tieneLike(int idPelicula) const {
    return likes.count(idPelicula) > 0;
}

const std::set<int>& UserData::getLikes() const {
    return likes;
}

void UserData::agregarVerMasTarde(int idPelicula) {
    verMasTarde.insert(idPelicula);
    notify();  // <-- NUEVO: notificar cambio
}

void UserData::quitarVerMasTarde(int idPelicula) {
    verMasTarde.erase(idPelicula);
    notify();  // <-- NUEVO: notificar cambio
}

bool UserData::estaEnVerMasTarde(int idPelicula) const {
    return verMasTarde.count(idPelicula) > 0;
}

const std::set<int>& UserData::getVerMasTarde() const {
    return verMasTarde;
}

// ============================================================
// PERSISTENCIA (SIN CAMBIOS)
// ============================================================

void UserData::guardarArchivo(const std::set<int>& datos, const std::string& ruta) const {
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cerr << "[UserData] No se pudo abrir para escritura: " << ruta << "\n";
        return;
    }
    for (int id : datos) {
        archivo << id << "\n";
    }
}

void UserData::cargarArchivo(std::set<int>& datos, const std::string& ruta) {
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