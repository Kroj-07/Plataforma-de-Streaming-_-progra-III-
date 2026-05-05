#ifndef PAGINADOR_H
#define PAGINADOR_H

#include <vector>
using namespace std;

class Paginador{
    private:
        int paginaActual;
        int totalPaginas;
        int resuñtadosPorPaginas;
        int totalResultados;

    public:
        Paginador( int totalResultados, int resultadosPorPagina=5);

        int getPaginaActual();
        int getTotalPaginas();
        int getInicio();
        int gteFin();
        bool haySiguiente();
        bool hayAnterior();

        void siguiente();
        void anterior();
        void irAPagina(int pagina);

};
#endif