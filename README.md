# Plataforma de Streaming - Programacion III (UTEC)

Proyecto final del curso. Sistema de busqueda y recomendacion de peliculas
implementado en C++20 con un Suffix Trie como estructura principal.

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
