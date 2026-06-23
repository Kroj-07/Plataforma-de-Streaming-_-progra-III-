#ifndef USERDATA_H
#define USERDATA_H

#include <set>
#include <string>
#include <vector>          // <-- NUEVO
#include "../ui/IObserver.h"  // <-- NUEVO (o forward declaration)

using namespace std;

class UserData {
public:
    UserData();

    // --- Métodos existentes ---
    void darLike(int idPelicula);
    void quitarLike(int idPelicula);
    bool tieneLike(int idPelicula) const;
    const set<int>& getLikes() const;

    void agregarVerMasTarde(int idPelicula);
    void quitarVerMasTarde(int idPelicula);
    bool estaEnVerMasTarde(int idPelicula) const;
    const set<int>& getVerMasTarde() const;

    void guardar() const;
    void cargar();

    // --- NUEVO: Métodos del patrón Observer ---
    void attach(IObserver* observer);    // Registrar un observer
    void detach(IObserver* observer);    // Eliminar un observer
    void notify();                        // Notificar a todos los observers

private:
    set<int> likes;
    set<int> verMasTarde;

    static const string ARCHIVO_LIKES;
    static const string ARCHIVO_VER_MAS_TARDE;

    // --- NUEVO: Lista de observers ---
    std::vector<IObserver*> observers;   // <-- NUEVO

    void guardarArchivo(const set<int>& datos, const string& ruta) const;
    void cargarArchivo(set<int>& datos, const string& ruta);
};

#endif