# 🎬 Plataforma de Streaming - Programación III (UTEC)

**Proyecto final del curso.** Sistema de búsqueda y recomendación de películas implementado en C++20 con un **Suffix Trie** como estructura principal.

---

## 📋 Tabla de Contenidos

1. [Descripción General](#descripción-general)
2. [Arquitectura del Sistema](#arquitectura-del-sistema)
3. [Estructura de Datos: Suffix Trie](#estructura-de-datos-suffix-trie)
4. [Preprocesamiento de Datos](#preprocesamiento-de-datos)
5. [Algoritmos de Búsqueda y Ranking](#algoritmos-de-búsqueda-y-ranking)
6. [Sistema de Recomendaciones](#sistema-de-recomendaciones)
7. [Patrones de Diseño Implementados](#patrones-de-diseño-implementados)
8. [Programación Paralela](#programación-paralela)
9. [Benchmarks y Eficiencia](#benchmarks-y-eficiencia)
10. [Instalación y Uso](#instalación-y-uso)
11. [Estructura del Proyecto](#estructura-del-proyecto)
12. [Referencias Bibliográficas](#referencias-bibliográficas)

---

## 📝 Descripción General

La plataforma permite a los usuarios:

- Buscar películas por **palabra, frase o subcadena** (ej: "bat" → "Batman").
- Filtrar por **tags**: director, género, actor.
- Ver el **detalle completo** de una película (sinopsis, director, casting).
- **Dar Like** a películas y guardarlas en **"Ver más tarde"**.
- Recibir **recomendaciones personalizadas** basadas en sus likes.

**Dataset:** El sistema carga un archivo CSV con más de **35,000 películas** (77MB) y las indexa en un Suffix Trie para búsquedas rápidas en **O(k)**.

---

## 🏗️ Arquitectura del Sistema

El programa está organizado en **cuatro capas** con responsabilidades bien definidas:

| Capa | Componentes | Responsabilidad |
|------|-------------|-----------------|
| **1. Interfaz de Usuario** | `MainMenu`, `SearchMenu`, `DetailMenu`, `WatchLaterMenu`, `RecommendMenu`, `Paginador` | Maneja la interacción con el usuario y la navegación. |
| **2. Lógica de la Aplicación** | `Buscador`, `Recommender`, `RelevanceScorer` | Orquesta las búsquedas, genera recomendaciones y calcula relevancia. |
| **3. Estructuras de Datos** | `Trie`, `TagIndex`, `Normalizador`, `Repository<T>` | Almacena y organiza los datos para acceso eficiente. |
| **4. Persistencia** | `CSVReader`, `UserData` | Lee el archivo CSV y guarda/ carga los datos del usuario. |

**Principio aplicado:** **Separación de Responsabilidades (SRP)**. Cada capa puede modificarse sin afectar a las demás.

---

## 🌳 Estructura de Datos: Suffix Trie

### ¿Qué es un Suffix Trie?

Un **Suffix Trie** es un árbol donde cada nodo representa un carácter. Insertamos **todos los sufijos** de cada palabra, lo que permite búsquedas por **subcadena** en **O(k)** (k = longitud de la consulta).

**Ejemplo:** Para la palabra `"batman"`, insertamos los sufijos: "batman", "atman", "tman", "man", "an", "n"

**Visualización:**
raíz
|
b
|
a
|
t
/
m (otros)
|
a
|
n


**Complejidades:**
| Operación | Complejidad | Explicación |
|-----------|-------------|-------------|
| **Búsqueda** | O(k) | Solo recorremos la longitud de la consulta. |
| **Inserción** | O(n²) | Insertamos todos los sufijos de cada palabra (n sufijos de longitud promedio n/2). |

**Limitación:** El espacio es O(n²) en el peor caso. Por eso **truncamos la sinopsis a 120 caracteres** para evitar problemas de memoria (el dataset tiene 35,000 películas con sinopsis largas).

---

## 🧹 Preprocesamiento de Datos

Antes de insertar en el Trie, los datos pasan por un **Normalizador** que:

1. **Convierte a minúsculas:** `"Batman"` → `"batman"`.
2. **Elimina signos de puntuación:** `"¡Hola!"` → `"hola"`.
3. **Elimina tildes y caracteres especiales:** `"acción"` → `"accion"` (pérdida de acentos).
4. **Tokeniza:** Divide el texto en palabras por espacios.

**Ejemplo real:**
| Entrada (CSV) | Después del Normalizador |
|---------------|--------------------------|
| `"The Dark Knight"` | `"the dark knight"` |
| `"Christopher Nolan"` | `"christopher nolan"` |
| `"Action, Drama"` | `"action drama"` |

**Limitación:** Se pierden acentos y la 'ñ' se convierte en 'n'. Es una decisión de diseño consciente para mantener el Trie simple y eficiente.

---

## 🔍 Algoritmos de Búsqueda y Ranking

### Búsqueda por Texto (Trie)

1. El usuario escribe una consulta (ej: `"nolan"`).
2. `Normalizador` la limpia: `"nolan"`.
3. `Trie::buscarFrase()` divide la consulta en palabras y busca cada una en el Trie.
4. Devuelve un **conjunto de IDs** (UNIÓN de resultados).

### Búsqueda por Tags (TagIndex)

- Índices en `unordered_map` para **director, género y actor**.
- Búsqueda **O(1)** promedio, pero solo por **coincidencia exacta** (ej: `"christopher nolan"` ≠ `"nolan"`).

### Algoritmo de Ranking (RelevanceScorer)

Ordena los resultados por relevancia usando **pesos fijos**:

| Campo | Peso | Razón |
|-------|------|-------|
| **Título** | 50 | La coincidencia en el título es la más relevante. |
| **Director / Casting** | 30 | Moderadamente relevante. |
| **Sinopsis (por aparición)** | 5 | Cada aparición suma poco, pero muchas apariciones suman. |
| **Bonus por año** | `anio / 100` | Las películas más recientes tienen una pequeña ventaja. |

**Fórmula:** 
Puntaje = (Título × 50) + (Director × 30) + (Casting × 30) + (Sinopsis × 5) + (anio / 100)


**Limitación:** Los pesos son arbitrarios. En el futuro, planeamos implementar **TF-IDF** para un ranking más preciso.

---

## 🎯 Sistema de Recomendaciones

**Algoritmo:** Basado en **matching exacto** de características entre películas likeadas y no likeadas.

| Factor | Peso | Descripción |
|--------|------|-------------|
| **Mismo Director** | +5 | Alta similitud (ej: Nolan). |
| **Mismo Género** | +3 | Moderada similitud (ej: Acción). |
| **Mismos Actores** | +2 | Baja similitud. |

**Ejemplo:** Si el usuario da Like a `"Interestelar"` (Christopher Nolan, Ciencia Ficción, Drama), el sistema recomendará `"Dunkirk"` (+5 por director) y `"Inception"` (+5 por director).

**Limitación:** Solo compara strings exactos. No tokeniza géneros ni actores (ej: `"accion, drama"` ≠ `"drama, accion"`). Mejora futura: tokenizar y comparar conjuntos.

---

## 🧩 Patrones de Diseño Implementados

| Patrón | Ubicación | Propósito |
|--------|-----------|-----------|
| **Factory** | `PeliculaFactory` | Centraliza la creación de objetos `Pelicula` desde el CSV. |
| **Strategy** | `RelevanceScorer` (Strategy Pattern) | Permite cambiar el algoritmo de ranking (lineal → TF-IDF) sin modificar el Buscador. (Implementado por Nicole). |
| **Observer** | `UserData` ↔ `MainMenu` | La UI se actualiza automáticamente cuando el usuario da Like o agrega a "Ver más tarde". |
| **Singleton** | `UserData` | Garantiza una única instancia de los datos del usuario en toda la aplicación. (Implementado por Thiago). |
| **Repository (Genérico)** | `Repository<T>` | Almacenamiento genérico con búsqueda por ID en O(1). |

**Justificación:** Estos patrones aplican los principios **SOLID** (Responsabilidad Única, Abierto/Cerrado, Inversión de Dependencias) y mejoran la mantenibilidad del código.

---

## ⚡ Programación Paralela

**Implementación:** Paralelización de la carga del CSV usando `std::async`.

| Método | Tiempo (segundos) | Mejora |
|--------|-------------------|--------|
| Secuencial | ~5.2s | Línea base |
| Paralelo (4 hilos) | ~1.8s | **65% más rápido** |

**Beneficio:** Mejora la experiencia de usuario al reducir el tiempo de inicio del programa.

---

## 📊 Benchmarks y Eficiencia

| Prueba | Tiempo (segundos) | Mejora |
|--------|-------------------|--------|
| **Carga CSV secuencial** | ~5.2s | Línea base |
| **Carga CSV paralela (4 hilos)** | ~1.8s | **65% más rápido** |
| **Búsqueda lineal (35,000 películas)** | ~0.045s | - |
| **Búsqueda con Trie (35,000 películas)** | ~0.0003s | **99% más rápido** |

**Conclusión:** El Suffix Trie reduce drásticamente el tiempo de búsqueda, y la carga paralela acelera el inicio del programa.

---

## 🛠️ Instalación y Uso

### Requisitos
- **C++20** (o superior).
- **CMake 3.10+**.
- **Compilador GCC/Clang/MSVC** con soporte C++20.

### Compilación

git clone https://github.com/Kroj-07
Plataforma-de-Streaming-_-progra-III-.git
cd Plataforma-de-Streaming-_-progra-III-
mkdir build && cd build
cmake ..
cmake --build .


### Ejecucion
./streaming_app

### Uso básico
Al iniciar, se muestra la pantalla de inicio con las listas de "Ver más tarde" y "Recomendaciones".

Presiona [1] para buscar una película (ej: "nolan").

Selecciona un número del [1-5] para ver el detalle de una película.

En el detalle, puedes:

[L] Like.

[V] Ver más tarde.

[Q] Quitar marcas.

Desde el menú principal, puedes acceder a [2] Ver más tarde y [3] Recomendaciones.

## Estructura del proyecto
Plataforma-de-Streaming-_-progra-III-/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── data/
│   ├── processed/peliculas_limpias.csv
│   └── raw/peliculas.csv
├── docs/
│   ├── informe_avance.pdf
│   └── user_flow_streaming.pdf
├── src/
│   ├── core/
│   │   ├── Normalizador.h
│   │   ├── Repository.h
│   │   ├── Trie.h
│   │   └── Trie.cpp
│   ├── data/
│   │   ├── CSVReader.h
│   │   ├── CSVReader.cpp
│   │   ├── Pelicula.h
│   │   ├── PeliculaFactory.h
│   │   ├── TagIndex.h
│   │   └── TagIndex.cpp
│   ├── search/
│   │   ├── Buscador.h
│   │   ├── Buscador.cpp
│   │   ├── RelevanceScorer.h
│   │   └── RelevanceScorer.cpp
│   ├── ui/
│   │   ├── IObserver.h
│   │   ├── MenuBase.h
│   │   ├── MainMenu.h
│   │   ├── MainMenu.cpp
│   │   ├── SearchMenu.h
│   │   ├── DetailMenu.h
│   │   ├── WatchLaterMenu.h
│   │   ├── RecommendMenu.h
│   │   ├── Paginador.h
│   │   └── Paginador.cpp
│   └── user/
│       ├── UserData.h
│       ├── UserData.cpp
│       ├── Recommender.h
│       └── Recommender.cpp
└── user_data/
    ├── likes.txt
    └── ver_mas_tarde.txt

## Referencias Bibliográficas
Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to Algorithms (3rd ed.). MIT Press. [Libro de referencia para estructuras de datos y complejidad temporal].

Stroustrup, B. (2013). The C++ Programming Language (4th ed.). Addison-Wesley. [Libro de referencia para el estándar C++ y programación genérica].

Gusfield, D. (1997). Algorithms on Strings, Trees, and Sequences. Cambridge University Press. [Referencia para Suffix Trees y Suffix Tries].

Gamma, E., Helm, R., Johnson, R., & Vlissides, J. (1994). Design Patterns: Elements of Reusable Object-Oriented Software. Addison-Wesley. [Libro de patrones de diseño (GoF)].

## Integrantes
Kiara Rojas M. – Interfaz de Usuario, Orquestación, Paginación, Patrones Factory y Observer, Documentación.

Nicole Ortega H. – Estructuras de Datos (Trie, Tags, Ranking), Patrón Strategy.

Thiago Frías P. – Persistencia, Recomendaciones, Programación Paralela, Patrón Singleton.