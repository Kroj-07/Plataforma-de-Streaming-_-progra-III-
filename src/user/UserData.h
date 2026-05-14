#ifndef USERDATA_H
#define USERDATA_H

#include <set>
#include <string>
using namespace std;

class UserData {
public:
    UserData();

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

private:
    set<int> likes;
    set<int> verMasTarde;

    static const string ARCHIVO_LIKES;
    static const string ARCHIVO_VER_MAS_TARDE;

    void guardarArchivo(const set<int>& datos, const string& ruta) const;
    void cargarArchivo(set<int>& datos, const string& ruta);
};

#endif