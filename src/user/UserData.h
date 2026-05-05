#ifndef USERDATA_H
#define USERDATA_H

#include <set>
#include <vector>
using namespace std;

class UserData {
public:
    UserData();
    
    // Cargar datos desde archivos
    void cargar();
    
    // Guardar datos en archivos
    void guardar();
    
    // Likes
    void darLike(int idPelicula);
    void quitarLike(int idPelicula);
    set<int> obtenerLikes();
    
    // Ver más tarde
    void agregarVerMasTarde(int idPelicula);
    void quitarVerMasTarde(int idPelicula);
    set<int> obtenerVerMasTarde();
    
private:
    set<int> likes;
    set<int> verMasTarde;
};

#endif