cmake_minimum_required(VERSION 3.10)
project(Plataforma-de-Streaming-_-progra-III-)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        src/core
        src/data
        src/search
        src/user
        src/ui
)

# 1. EL EJECUTABLE PRINCIPAL (Corregido)
# Tienes que listar todos los archivos .cpp que tienen lógica
add_executable(streaming_app
        src/main.cpp
        src/core/Trie.cpp
        src/core/UserData.cpp
        # src/ui/Menu.cpp  <-- (Descomenta esto si ya tienen un Menu.cpp)
)

# 2. EL EJECUTABLE DE PRUEBAS
add_executable(test_trie
        test/test_trie.cpp
        src/core/Trie.cpp
)

# Crear carpeta de persistencia
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/user_data)

message(STATUS "Targets disponibles: streaming_app  |  test_trie")