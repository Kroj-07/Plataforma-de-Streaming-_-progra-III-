# Plataforma de Streaming - Programacion III (UTEC)

Proyecto final del curso. Sistema de busqueda y recomendacion de peliculas
implementado en C++20 con un Suffix Trie como estructura principal.

## Integrantes y responsabilidades

| Integrante | Rol | Modulos |
|---|---|---|
| A | Estructura de datos | `src/core/Trie.*`, `src/core/Normalizador.h` |
| B | Datos y relevancia | `src/data/*`, `src/search/RelevanceScorer.h` |
| C | Usuario y recomendaciones | `src/user/UserData.*`, `src/user/Recommender.*` |
| D | Interfaz y orquestacion | `main.cpp`, `src/ui/*`, `src/search/Buscador.*` |

## Estructura

```
ProyectoFinal-Streaming/
├── main.cpp                  # Punto de entrada (D)
├── CMakeLists.txt            # Build automatizado
├── data/
│   ├── raw/peliculas.csv     # Dataset original
│   └── processed/            # Dataset normalizado (B)
├── docs/                     # Informes y pseudocodigos
├── src/
│   ├── core/                 # Trie y Normalizador (A)
│   ├── data/                 # CSVReader, TagIndex, Pelicula (B)
│   ├── search/               # Buscador y RelevanceScorer (A+B)
│   ├── user/                 # UserData y Recommender (C)
│   └── ui/                   # Menu y Paginador (D)
├── test/                     # Pruebas unitarias
└── user_data/                # Persistencia (likes, ver_mas_tarde)
```

## Como compilar

Requisitos: CMake >= 3.10 y un compilador C++20 (GCC 10+, Clang 11+, MSVC 2019+).

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Esto produce dos binarios:

- `streaming_app` - programa principal con el menu interactivo.
- `test_trie` - suite de pruebas del Trie.

## Como ejecutar

Desde la raiz del proyecto (no desde `build/`, porque el CSV se lee con ruta relativa):

```bash
./build/streaming_app
```

Si `data/processed/peliculas_limpias.csv` esta vacio o no existe, el
programa carga 12 peliculas simuladas para que la demo funcione siempre.

## Menu

1. **Buscar** - texto libre, busqueda por subcadena (Suffix Trie), resultados
   ordenados por relevancia, paginacion de 5 en 5.
2. **Ver mas tarde** - lista persistente de peliculas guardadas.
3. **Recomendaciones** - basadas en likes (overlap de genero + director).
4. **Salir** - guarda likes y "ver mas tarde" en `user_data/`.

## Avance Semana 8

Esta entrega incluye:

- Codigo C++ compilable (todos los modulos cierran sus stubs).
- Pseudocodigos de insercion y busqueda en `docs/`.
- Manual de usuario y diagrama de navegacion.
- Pre-procesamiento del CSV.
