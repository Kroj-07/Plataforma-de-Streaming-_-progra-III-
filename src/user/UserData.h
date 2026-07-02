#ifndef USERDATA_H
#define USERDATA_H

#include <set>
#include <string>
#include <vector>          // <-- NUEVO
#include "../ui/IObserver.h"  // <-- NUEVO (o forward declaration)

// Patron Singleton: existe una unica instancia global de los datos del
// usuario (likes / ver-mas-tarde), accesible desde cualquier parte del
// programa via getInstance(). El constructor es privado y la copia/asignacion
// estan deshabilitadas para garantizar que no se puedan crear instancias
// adicionales por accidente.
class UserData {
public:
    static UserData& getInstance();

    UserData(const UserData&) = delete;
    UserData& operator=(const UserData&) = delete;

    // --- Métodos existentes ---
    void darLike(int idPelicula);
    void quitarLike(int idPelicula);
    bool tieneLike(int idPelicula) const;
    const std::set<int>& getLikes() const;

    void agregarVerMasTarde(int idPelicula);
    void quitarVerMasTarde(int idPelicula);
    bool estaEnVerMasTarde(int idPelicula) const;
    const std::set<int>& getVerMasTarde() const;

    void guardar() const;
    void cargar();

    // --- NUEVO: Métodos del patrón Observer ---
    void attach(IObserver* observer);    // Registrar un observer
    void detach(IObserver* observer);    // Eliminar un observer
    void notify();                        // Notificar a todos los observers

private:
    UserData();  // Constructor privado: usar getInstance().

    std::set<int> likes;
    std::set<int> verMasTarde;

    static const std::string ARCHIVO_LIKES;
    static const std::string ARCHIVO_VER_MAS_TARDE;

    // --- NUEVO: Lista de observers ---
    std::vector<IObserver*> observers;   // <-- NUEVO

    void guardarArchivo(const std::set<int>& datos, const std::string& ruta) const;
    void cargarArchivo(std::set<int>& datos, const std::string& ruta);
};

#endif