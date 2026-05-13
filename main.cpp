#include <iostream>
#include <vector>
#include <string>
#include<algorithm>
#include <limits>

using namespace std;


//==========ESTRUCTUTRA TEMPORAL==========
struct Pelicula{
    int id;
    string titulo;
    string sinopsis;
    string director;
    vector<string> generos;
    int año;

};


//========== BASE DE DATOS SIMULADA ==========
vector<Pelicula>peliculasDB= {
    {1,"Iron Man", "Un multimillorio construye un traje blindado.","Jon Favreau",
        {"Accion", " Ciencia Ficcion"}, 2008},
    {2, "Iron Man 2", "Tony Stark enfrenta a Whiplash.","Shane Black",
        {"Accion", "Ciencia Ficcion"},2010},
     {3, "Iron Man 3", "Tony Stark enfrenta al Mandarín.", "Shane Black",
        {"Accion", "Ciencia Ficción"}, 2013},
    {4, "Titanic", "Historia de amor en el barco que se hunde.", "James Cameron",
        {"Drama", "Romance"}, 1997},
    {5, "Avatar", "Marine viaja a Pandora.", "James Cameron",
        {"Accion", "Ciencia Ficción"}, 2009},
    {6, "El Principito", "Un piloto conoce a un pequeño príncipe.", "Mark Osborne",
        {"Animación", "Fantasía"}, 2015},
    {7, "Batman Inicia", "Bruce Wayne se convierte en Batman.", "Christopher Nolan",
        {"Accion", "Drama"}, 2005},
    {8, "Batman: El Caballero de la Noche", "Batman enfrenta al Joker.", "Christopher Nolan",
        {"Accion", "Drama"}, 2008},
    {9, "Interestelar", "Astronautas buscan nuevo hogar.", "Christopher Nolan",
        {"Ciencia Ficción", "Drama"}, 2014},
    {10, "El Origen", "Ladrones de sueños.", "Christopher Nolan",
        {"Accion", "Ciencia Ficción"}, 2010},
    {11, "Matrix","La realidad es una simulacion.","Hermana Wachowski",
        {"Accion", "Ciencia Ficcion"}, 1999},
    {12, "Volver al futuro", "Un joven viaja en el tiempo","Robert Zemckis",
        {"Aventura", "Ciencia Ficcion"}, 1985}
};


//========== DATOS DE USUARIOS SIMULADOS ==========
vector<int> verMasTardeIDs={1,4,10};
vector<int> likesIDs={5,9,11};


//========== FUNCIONES AUXILIARES ==========

void limpiarPantalla(){
    #ifdef _WIN32
        system("cls");
    #else
        system ("clear");
    #endif
}

void pausar(){
    cout<<"Presione Enter apra ocntinuar..."<<endl;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.get();
}

Pelicula* buscarPeliculaPorId(int id){
    for (auto& p:peliculasDB){
        if (p.id==id) return &p;

    }
    return nullptr;
}

string aMinusculas(string texto){
    string resultado =texto;
    for (char& c:resultado) c=tolower(c);
    return resultado;
}


//========== FUNCION DE BUSQUEDA SIMULADA ==========
vector<Pelicula*> buscarPeliculas(string query){
    vector<Pelicula*> resultados;
    string queryLower=aMinusculas(query);

    for(auto& p:peliculasDB){
        string tituloLower = aMinusculas(p.titulo);
        string sinopsisLower = aMinusculas(p.sinopsis);

        //Buscar en titulo o sinopsis
        if (tituloLower.find(queryLower) != string::npos || 
            sinopsisLower.find(queryLower) != string::npos){
            resultados.push_back(&p);
        }
    }
    return resultados;
}

//========== PAGINADOR ==========
struct Pagina {
    int paginaActual;
    int totalPaginas;
    int resultadosPorPagina;
    int totalResultados;

    Pagina(int total, int porPagina=5){
        totalResultados=total;
        resultadosPorPagina=porPagina;
        paginaActual=1;
        totalPaginas=(total+porPagina-1)/porPagina;
        if (totalPaginas==0) totalPaginas=1;
    }

    int inicio(){ return (paginaActual -1)*resultadosPorPagina;}
    int fin() {return min(inicio() +resultadosPorPagina, totalResultados);}

    bool haySiguiente(){return paginaActual < totalPaginas;}
    bool hayAnterior(){ return paginaActual>1;}

    void siguiente(){ if (haySiguiente())paginaActual++;}
    void anterior(){ if (hayAnterior()) paginaActual--;}

};


//========== PANTALLAS DEL MENU ==========
void mostrarDetallePelicula(int id){
    Pelicula* p=buscarPeliculaPorId(id);
    if(!p){
        cout<<"Pelicula no encontrada."<<endl;
        pausar();
        return;
    }

    limpiarPantalla();
    cout<<"══════════════════════════════════════════════════\n";
    cout<<" "<<p->titulo<< " ("<<p->año<<")\n";
    cout<<"══════════════════════════════════════════════════\n\n";
    cout<<"Director: "<< p->director<< "\n";
    cout<<"Generos: ";
    for (size_t i=0; i<p->generos.size(); i++){
        cout<< p->generos[i];
        if (i<p->generos.size() -1) cout<<", ";
    }
    cout << "\n\n";
    cout << "Sinopsis:\n" << p->sinopsis << "\n\n";
    cout << "══════════════════════════════════════════════════\n";
    cout << "  [L] Dar Like    [V] Ver más tarde    [Enter] Volver\n";
    cout << "  Seleccione: ";

    string opcion;
    getline(cin, opcion);

    if (opcion == "L" or opcion=="l"){
        //Verificar si ya tiene like
        if(find(likesIDs.begin(), likesIDs.end(), id) != likesIDs.end()){
            cout<<"Ya le diste Like a esta pelicula."<<endl;
        } else{
            likesIDs.push_back(id);
            cout<<">> ¡Like agregado! (simulado)"<<endl;
        }
    }else if (opcion=="V" or opcion=="v"){
        if (find(verMasTardeIDs.begin(), verMasTardeIDs.end(),id)!= verMasTardeIDs.end()){
            cout<<">> Ya esta en 'Ver mas tarde'."<<endl;

        }else{
            verMasTardeIDs.push_back(id);
            cout<<">> Agregado a 'Ver mas tarde'"<<endl;

            }
        }
    pausar();
}

void menuBuscar() {
    limpiarPantalla();
    cout << "══════════════════════════════\n";
    cout << "  BUSCAR PELÍCULA\n";
    cout << "══════════════════════════════\n";
    cout << "Puede buscar por palabra, frase o parte de una palabra.\n";
    cout << "Ejemplos: 'iron', 'barco fantasma', 'bat'\n\n";
    cout << "Ingrese su búsqueda: ";
    
    string busqueda;
    getline(cin,busqueda);

    if (busqueda.empty()) return;
    
    // Buscar
    vector<Pelicula*> resultados = buscarPeliculas(busqueda);

    if (resultados.empty()){
        cout << "\nNo se encontraron coincidencias para: \"" << busqueda << "\"\n";
        pausar();
        return;
    }

    //Paginacion
    Pagina pag(resultados.size(), 5);
    char navegacion;

    do{
        limpiarPantalla();
        cout << "══════════════════════════════════════════\n";
        cout << "  Resultados para: \"" << busqueda << "\"\n";
        cout << "══════════════════════════════════════════\n\n";
        
        // Mostrar películas de la página actual
        for (int i=pag.inicio(); i<pag.fin(); i++){
            cout<<" ["<<(i-pag.inicio()+1)<<"] "
                <<resultados[i]->titulo
                <<" ("<< resultados[i]->año<<")\n";
            cout<<"     "<< resultados[i]->director<<"\n\n";
        }

        //Informacion de paginacion
        cout << "──────────────────────────────────────────\n";
        cout << "  Página " << pag.paginaActual << " de " << pag.totalPaginas;
        cout << "  (Mostrando " << (pag.fin() - pag.inicio()) << " de " << pag.totalResultados << ")\n";
        cout << "──────────────────────────────────────────\n";
        cout << "  [1-5] Ver detalle  ";
        if (pag.hayAnterior()) cout << "[A] Anterior  ";
        if (pag.haySiguiente()) cout << "[S] Siguiente  ";
        cout << "[V] Volver\n";
        cout << "  Seleccione: ";

        cin>>navegacion;
        cin.ignore();

        if (navegacion>='1' and navegacion<='5'){
            int indice= pag.inicio()+ (navegacion-'1');
            if (indice < pag.totalResultados){
                mostrarDetallePelicula(resultados[indice]->id);
            }
        }else if((navegacion=='S' or navegacion=='s') and pag.haySiguiente()){
            pag.siguiente();

        }else if((navegacion == 'A' || navegacion == 'a') && pag.hayAnterior()){
            pag.anterior();
        }
            
    }while (navegacion != 'V' and navegacion !='v');

}

void menuVerMasTarde(){
    limpiarPantalla();
    cout << "══════════════════════════════\n";
    cout << "  VER MÁS TARDE\n";
    cout << "══════════════════════════════\n\n";
    
    if (verMasTardeIDs.empty()) {
        cout << "No tienes películas guardadas en 'Ver más tarde'.\n\n";
        cout << "Puedes agregar películas cuando busques y selecciones una.\n";
        pausar();
        return;
    }

    for (size_t i =0; i < verMasTardeIDs.size();i++){
        Pelicula* p= buscarPeliculaPorId(verMasTardeIDs[i]);
        if(p) {
            cout <<"  [" << (i+1) << "] " << p->titulo << " (" << p->año << ")\n";
            cout << "      " << p->director << "\n\n";
        }
    }

     cout << "──────────────────────────────────────────\n";
    cout << "  Seleccione un número para ver detalle, o [V] para volver: ";
    
    string opcion;
    getline(cin, opcion);

    if (opcion=="V" or opcion=="v") return;
    try{
        int indice =stoi(opcion);
        if (indice >= 1 and indice<= (int)verMasTardeIDs.size()){
            mostrarDetallePelicula(verMasTardeIDs[indice-1]);
        }
    }catch (...){
        //input invalido, volver
    }
}

void menuRecomendaciones(){
    limpiarPantalla();
    cout << "══════════════════════════════\n";
    cout << "  RECOMENDACIONES PARA TI\n";
    cout << "══════════════════════════════\n";
    cout << "  Basado en tus Likes\n\n";
    
    if (likesIDs.empty()) {
        cout << "Aún no has dado Like a ninguna película.\n";
        cout << "Busca películas y dales Like para recibir recomendaciones.\n";
        pausar();
        return;
}


// Algoritmo simple simulado: buscar peliculas con géneros similares
// a las likeadas, excluyendo las ya likeadas y las de ver más tarde

int mostradas=0;
for(auto& p: peliculasDB){
    //saltar si ya tiene like
    if (find(likesIDs.begin(), likesIDs.end(), p.id)!= likesIDs.end())continue;

    //verificar similitud con alguna pelicula likeada
    bool recomendada=false;
    string razon="";

    for (int idLike:likesIDs){
        Pelicula* pLike=buscarPeliculaPorId(idLike);
        if(!pLike) continue;

        //Mismo director
        if(p.director == pLike->director){
            recomendada=true;
            razon="Mismo director: "+p.director;
            break;
        }

        //generos en comun
        for(auto& g: p.generos){
            if (find(pLike->generos.begin(), pLike->generos.end(), g)
            != pLike->generos.end()){
                recomendada= true;
                razon="Genero similar: "+g;
            break;}
        }
        if (recomendada) break;

    }
    if(recomendada){
        mostradas++;
        cout << "  ★ " << p.titulo << " (" << p.año << ")\n";
        cout << "    " << razon << "\n";
        cout << "    Director: " << p.director << "\n\n";

        if (mostradas==5) break;
    }
}

if (mostradas==0){
    cout<<"No hay recomendaciones disponibles aún."<<endl;
}
pausar();
}

// ==========PANTALLA DE INICIO ==========
void PantallaInicio(){
    limpiarPantalla();
    cout << "╔══════════════════════════════════════════╗\n";
    cout << "║   PLATAFORMA DE STREAMING               ║\n";
    cout << "║   Proyecto Final - Programación III      ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";
    
    // Mostrar "Ver más tarde"
    cout << "──────────────────────────────────────────\n";
    cout << "  📌 VER MÁS TARDE\n";
    cout << "──────────────────────────────────────────\n";
    if (verMasTardeIDs.empty()) {
        cout << "  No tienes películas guardadas.\n";
    } else{
        for (int id:verMasTardeIDs){
            Pelicula* p=buscarPeliculaPorId(id);
            if (p) cout <<"  • " << p->titulo << " (" << p->año << ")\n";
        }
    }
    //Mostrar recomendaciones basadas en likes
    cout << "\n──────────────────────────────────────────\n";
    cout << "  ❤️ RECOMENDACIONES BASADAS EN TUS LIKES\n";
    cout << "──────────────────────────────────────────\n";
    if (likesIDs.empty()) {
        cout << "  Da Like a películas para recibir recomendaciones.\n";
    } else {
        //Misma logica simplificada
        int mostrada=0;
        for (auto& p: peliculasDB){
            if(find(likesIDs.begin(), likesIDs.end(),p.id)!= likesIDs.end()) continue;
        }
    }
}

// ========== MENÚ PRINCIPAL ==========
int main() {
    int opcion;
    
    // Mostrar pantalla de inicio
    PantallaInicio();
    
        do {
        limpiarPantalla();
        cout << "========================================\n";
        cout << "         MENU PRINCIPAL                \n";
        cout << "========================================\n";
        cout << "  1. Buscar pelicula                   \n";
        cout << "  2. Ver 'Ver mas tarde'               \n";
        cout << "  3. Ver recomendaciones               \n";
        cout << "  4. Salir                             \n";
        cout << "========================================\n";
        cout << "  Seleccione una opcion: ";
        
        
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1: menuBuscar(); break;
            case 2: menuVerMasTarde(); break;
            case 3: menuRecomendaciones(); break;
            case 4:
                cout << "\n  Guardando datos...\n";
                cout << "  ¡Hasta luego!\n";
                break;
            default:
                cout << "\n  Opcion invalida.\n";
                pausar();
        }
    } while (opcion != 4);
    
    return 0;
}