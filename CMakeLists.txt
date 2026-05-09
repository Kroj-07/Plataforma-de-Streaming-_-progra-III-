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

add_executable(streaming_app
        main.cpp
        src/core/Trie.cpp
        src/core/UserData.cpp
)

add_executable(test_trie
        test/test_trie.cpp
        src/core/Trie.cpp
)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/user_data)

message(STATUS "Targets disponibles: streaming_app  |  test_trie")