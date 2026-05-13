#ifndef USERDATA_H
#define USERDATA_H

#include <set>
#include <string>
using namespace std;

// Persistencia de datos del usuario (Integrante C).
// Likes y "Ver más tarde" se guardan como IDs en archivos de texto.
class UserData {
public:
    UserData();

    void darLike(int idPelicula);
    void quitarLike(int idPelicula);
    bool tieneLike(int idPelicula) const;
    const std::set<int>& getLikes() const;

    void agregarVerMasTarde(int idPelicula);
    void quitarVerMasTarde(int idPelicula);
    bool estaEnVerMasTarde(int idPelicula) const;
    const std::set<int>& getVerMasTarde() const;
    void    darLike(int idPelicula);
    void    quitarLike(int idPelicula);
    bool    tieneLike(int idPelicula) const;
    const set<int>& getLikes() const;

    void    agregarVerMasTarde(int idPelicula);
    void    quitarVerMasTarde(int idPelicula);
    bool    estaEnVerMasTarde(int idPelicula) const;
    const set<int>& getVerMasTarde() const;

    void guardar() const;
    void cargar();

private:
    std::set<int> likes;
    std::set<int> verMasTarde;

    static const std::string ARCHIVO_LIKES;
    static const std::string ARCHIVO_VER_MAS_TARDE;

    void guardarArchivo(const std::set<int>& datos, const std::string& ruta) const;
    void cargarArchivo(std::set<int>& datos, const std::string& ruta);
};

#endif
    set<int> likes;
    set<int> verMasTarde;

    static const string ARCHIVO_LIKES;
    static const string ARCHIVO_VER_MAS_TARDE;

    void guardarArchivo(const set<int>& datos, const string& ruta) const;
    void cargarArchivo(set<int>& datos, const string& ruta);
};

#endif // USERDATA_H
