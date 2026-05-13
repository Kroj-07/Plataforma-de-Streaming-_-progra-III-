#ifndef PAGINADOR_H
#define PAGINADOR_H

// Lógica de paginación 5-en-5 (Integrante D).
// Maneja índices [inicio, fin) sin tocar los datos paginados.
class Paginador {
private:
    int paginaActual;
    int totalPaginas;
    int resultadosPorPagina;
    int totalResultados;

public:
    Paginador(int totalResultados, int resultadosPorPagina = 5);

    int  getPaginaActual()    const;
    int  getTotalPaginas()    const;
    int  getResultadosPorPag()const;
    int  getTotalResultados() const;
    int  getInicio()          const;   // índice del primer resultado de la página actual
    int  getFin()             const;   // índice (exclusivo) del fin de la página actual

    bool haySiguiente() const;
    bool hayAnterior()  const;

    void siguiente();
    void anterior();
    void irAPagina(int pagina);
};

#endif
