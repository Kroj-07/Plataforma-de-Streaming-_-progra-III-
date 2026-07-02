cmake_minimum_required(VERSION 3.10)
project(Plataforma-de-Streaming)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        .
        src/core
        src/data
        src/search
        src/user
        src/ui
)

add_executable(streaming_app
        main.cpp
        src/core/Trie.cpp
        src/data/CSVReader.cpp
        src/data/TagIndex.cpp
        src/search/Buscador.cpp
        src/ui/Paginador.cpp
        src/user/Recommender.cpp
        src/user/UserData.cpp
        src/ui/MainMenu.cpp      # <-- Nuevo
)

# Carpeta donde UserData persiste likes / ver-mas-tarde.
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/user_data)

# Copia automatica del CSV al directorio de build, asi el ejecutable
# encuentra "data/processed/peliculas_limpias.csv" con ruta relativa
# sin importar si CLion lanza desde cmake-build-debug o desde el root.
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
