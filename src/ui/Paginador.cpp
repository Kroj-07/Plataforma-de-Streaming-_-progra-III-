#include "Paginador.h"
#include <algorithm>

Paginador::Paginador(int totalResultados, int resultadosPorPagina)
    : paginaActual(1)
    , resultadosPorPagina(resultadosPorPagina)
    , totalResultados(totalResultados)
{
    totalPaginas = (totalResultados + resultadosPorPagina - 1) / resultadosPorPagina;
    if (totalPaginas == 0) totalPaginas = 1;
}

int Paginador::getPaginaActual()    const { return paginaActual; }
int Paginador::getTotalPaginas()    const { return totalPaginas; }
int Paginador::getResultadosPorPag()const { return resultadosPorPagina; }
int Paginador::getTotalResultados() const { return totalResultados; }
int Paginador::getInicio()          const { return (paginaActual - 1) * resultadosPorPagina; }
int Paginador::getFin()             const { return std::min(getInicio() + resultadosPorPagina, totalResultados); }

bool Paginador::haySiguiente() const { return paginaActual < totalPaginas; }
bool Paginador::hayAnterior()  const { return paginaActual > 1; }

void Paginador::siguiente() { if (haySiguiente()) paginaActual++; }
void Paginador::anterior()  { if (hayAnterior())  paginaActual--; }
void Paginador::irAPagina(int pagina) {
    if (pagina >= 1 && pagina <= totalPaginas) paginaActual = pagina;
}
