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
| **1. Interfaz de Usuario** | `MainMenu`, `SearchMenu`, `TagSearchMenu`, `DetailMenu`, `WatchLaterMenu`, `RecommendMenu`, `LikesMenu`, `Paginador` | Maneja la interacción con el usuario y la navegación. |
| **2. Lógica de la Aplicación** | `Buscador`, `Recommender`, `IScoringStrategy`/`PesosFijosStrategy` | Orquesta las búsquedas, genera recomendaciones y calcula relevancia (ranking intercambiable vía Strategy). |
| **3. Estructuras de Datos** | `Trie`, `TagIndex`, `Normalizador`, `Repository<T>` | Almacena y organiza los datos para acceso eficiente. |
| **4. Persistencia** | `CSVReader`, `UserData` | Lee el archivo CSV y guarda/ carga los datos del usuario. |

**Principio aplicado:** **Separación de Responsabilidades (SRP)**. Cada capa puede modificarse sin afectar a las demás.

---

## 🌳 Estructura de Datos: Suffix Trie

### ¿Qué es un Suffix Trie?

Un **Suffix Trie** es un árbol donde cada nodo representa un carácter. Insertamos **todos los sufijos** de cada palabra, lo que permite búsquedas por **subcadena** en **O(k)** (k = longitud de la consulta).

**Ejemplo:** Para la palabra `"batman"`, insertamos los sufijos: "batman", "atman", "tman", "man", "an", "n"

**Visualización:**
```
raíz
 |
 b
 |
 a
 |
 t --- m (otros sufijos)
       |
       a
       |
       n
```

**Complejidades:**
| Operación | Complejidad | Explicación |
|-----------|-------------|-------------|
| **Búsqueda** | O(k) | Solo recorremos la longitud de la consulta. |
| **Inserción** | O(n²) | Insertamos todos los sufijos de cada palabra (n sufijos de longitud promedio n/2). |

**Limitación:** El espacio es O(n²) en el peor caso. Por eso **truncamos la sinopsis a 120 caracteres** para evitar problemas de memoria (el dataset tiene 35,000 películas con sinopsis largas).



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

### Algoritmo de Ranking (Strategy: `IScoringStrategy`)

El ranking se implementa con el **patrón Strategy**, de modo que se puede intercambiar el algoritmo sin tocar el `Buscador`. Hay **dos estrategias concretas**:

#### 1. `PesosFijosStrategy` (pesos fijos)

| Criterio | Peso | Razón |
|----------|------|-------|
| **Frase completa en el título** | +200 | Un match exacto de la consulta en el título es lo más relevante (ej: "iron man" → *Iron Man*). |
| **Término en el título** | +50 | Coincidencia de una palabra en el título. |
| **Término en director / casting** | +30 c/u | Moderadamente relevante. |
| **Término en género** | +10 | Poco relevante. |
| **Término presente en la sinopsis** | +5 | Se cuenta **presencia** (una vez), no por cada aparición. |
| **Bonus por año** | `anio / 100` | Las películas más recientes tienen una pequeña ventaja. |

**Decisión de diseño:** la sinopsis se puntúa por *presencia* y no por *cada aparición*. Antes se sumaba por aparición, lo que inflaba películas cuya sinopsis repetía subcadenas comunes (ej: "man" dentro de "many", "woman", "human"), dejándolas por encima de coincidencias exactas de título. El bonus por frase completa garantiza que el match exacto siempre domine.

#### 2. `TFIDFScoringStrategy` (TF-IDF ponderado) — **estrategia activa por defecto**

Pondera cada término por **TF-IDF**: un término que aparece en muchas películas (ej. "the", "man") pesa menos que uno raro y específico, gracias al **IDF** (Inverse Document Frequency) calculado sobre las 34,886 películas:

- **TF** (Term Frequency): cuenta las ocurrencias del término, ponderadas por campo (título ×10, director ×5, casting ×4, género ×2, sinopsis por presencia).
- **IDF**: `log10(1 + total_documentos / (1 + df))`, con suavizado de Laplace para evitar división por cero.
- **Bonus de +500** si el título contiene la frase completa de la búsqueda.

El `Buscador` se construye antes de cargar el CSV, así que su estrategia inicial no tiene datos. Por eso `MainMenu` **reconstruye** la estrategia TF-IDF con `buscador.setEstrategia(...)` justo después de cargar las 34,886 películas, para que el índice de frecuencias (DF) se calcule correctamente.

**Cómo cambiar de estrategia:** basta con llamar `buscador.setEstrategia(std::make_unique<PesosFijosStrategy>())` (o cualquier otra clase que implemente `IScoringStrategy`) — el `Buscador` no necesita cambios.

---

## 🎯 Sistema de Recomendaciones

**Algoritmo:** Basado en **matching exacto** de características entre películas likeadas y no likeadas.

| Factor | Peso | Descripción |
|--------|------|-------------|
| **Mismo Director** | +5 | Alta similitud (ej: Nolan). |
| **Mismo Género** | +3 | Moderada similitud (ej: Acción). |
| **Mismos Actores** | +2 | Baja similitud. |

**Ejemplo:** Si el usuario da Like a `"Interestelar"` (Christopher Nolan, Ciencia Ficción, Drama), el sistema recomendará `"Dunkirk"` (+5 por director) y `"Inception"` (+5 por director).

**Optimización aplicada:** El algoritmo **tokeniza** géneros y casting (por comas) y compara por **intersección de conjuntos**, de modo que `"accion, drama"` y `"drama, accion"` sí se consideran similares. Además, se precalcula un mapa `id → Pelicula*` una sola vez, reduciendo la complejidad de **O(P²·L)** a **O(P·L)** (medido: ~16× más rápido sobre 35 000 películas).

---

## 🧩 Patrones de Diseño Implementados

| Patrón | Ubicación | Propósito |
|--------|-----------|-----------|
| **Factory** | `PeliculaFactory` | Centraliza la creación de objetos `Pelicula` desde el CSV. |
| **Observer** | `UserData` (Subject) ↔ `MainMenu` (`IObserver`) | La UI se marca como desactualizada automáticamente cuando el usuario da Like o agrega a "Ver más tarde". |
| **Singleton** | `UserData::getInstance()` | Garantiza una única instancia global de los datos del usuario (Meyer's Singleton, thread-safe). Constructor privado + copia deshabilitada. |
| **Strategy** | `IScoringStrategy` → `PesosFijosStrategy` / `TFIDFScoringStrategy`, usado por `Buscador` | Permite intercambiar el algoritmo de ranking (pesos fijos ↔ TF-IDF) en tiempo de ejecución sin modificar el `Buscador` (`setEstrategia`). |
| **Repository (Genérico)** | `Repository<T>` | Almacenamiento genérico con búsqueda por ID en O(1) promedio. |

**Justificación:** Estos 5 patrones aplican los principios **SOLID** (Responsabilidad Única, Abierto/Cerrado, Inversión de Dependencias) y mejoran la mantenibilidad del código.

---

## ⚡ Programación Paralela

**Implementación:** Paralelización del **parseo y normalización** del CSV usando `std::async` (`CSVReader::cargarDatosParalelo`).

**¿Cómo funciona?**

1. La **lectura del archivo (E/S)** se hace en una sola pasada secuencial (leer disco no se beneficia de varios hilos).
2. Las líneas crudas se reparten en **N bloques** (N = `std::thread::hardware_concurrency()`).
3. Cada bloque se **parsea y normaliza en un hilo distinto** con `std::async(std::launch::async, ...)`. Esta es la parte costosa (CPU): tokenizar, quitar tildes/puntuación y pasar a minúsculas ~35 000 filas × 5 campos.
4. Los resultados se recogen **en orden**, se reasignan IDs contiguos y se llena el `TagIndex` en un solo hilo (porque `unordered_map` no es seguro para escritura concurrente).

**Medición real** (dataset de 34 886 películas, compilado con `-O2`, CPU de 12 núcleos):

| Método | Tiempo (promedio de 5 corridas) | Speedup |
|--------|--------------------------------|---------|
| Secuencial (`cargarDatos`) | ~2002 ms | 1.0x (línea base) |
| Paralelo (`cargarDatosParalelo`, 12 hilos) | ~773 ms | **2.59x más rápido** |

> Los tiempos se obtienen ejecutando [`bench_carga.cpp`](bench_carga.cpp), que compara ambas versiones sobre el mismo dataset. El speedup no es lineal (2.59x con 12 hilos) porque la E/S del archivo y el llenado del `TagIndex` siguen siendo secuenciales (ley de Amdahl).

**Beneficio:** Reduce a menos de la mitad el tiempo de inicio del programa.

---

## 📊 Benchmarks y Eficiencia

| Prueba | Tiempo | Mejora |
|--------|--------|--------|
| **Carga CSV secuencial** | ~2002 ms | Línea base |
| **Carga CSV paralela (12 hilos)** | ~773 ms | **2.59x más rápido** |

| **Indexado Trie CON truncamiento** | ~15.7 s | Línea base |
| **Indexado Trie SIN truncamiento** | ~610.8 s | 39x más lento |

**Conclusión:** La carga paralela acelera el inicio del programa a ~1/2.6 del tiempo, el Suffix Trie permite búsquedas por subcadena en O(k), y truncar la sinopsis a 120 caracteres evita el costo cuadrático de indexar texto completo (verificado empíricamente: 39x más lento sin truncar).

> Nota: los tiempos se miden con [`bench_carga.cpp`](bench_carga.cpp) y [`bench_trie.cpp`](bench_trie.cpp) sobre el dataset real de 34 886 películas.

---

## 🛠️ Instalación y Uso

### Requisitos
- **C++20** (o superior).
- **CMake 3.10+**.
- **Compilador GCC/Clang/MSVC** con soporte C++20.

### Compilación (con CMake)

```bash
git clone https://github.com/Kroj-07/Plataforma-de-Streaming-_-progra-III-.git
cd Plataforma-de-Streaming-_-progra-III-
mkdir build && cd build
cmake ..
cmake --build .
```

### Ejecución

```bash
./streaming_app
```

### Uso básico
Al iniciar, se muestra la pantalla de inicio con las listas de "Ver más tarde" y "Recomendaciones".

Presiona [1] para buscar una película (ej: "nolan").

Selecciona un número del [1-5] para ver el detalle de una película.

En el detalle, puedes:

[L] Like.

[V] Ver más tarde.

[Q] Quitar marcas.

Desde el menú principal puedes acceder a: [1] Buscar por texto, [2] Buscar por tag, [3] Ver más tarde, [4] Recomendaciones y [5] Mis likes.

## Estructura del proyecto
```
Plataforma-de-Streaming-_-progra-III-/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── bench_carga.cpp            # Benchmark carga secuencial vs. paralela
├── bench_trie.cpp             # Benchmark indexado Trie: con vs. sin truncar
├── test_recommender.cpp       # Test del algoritmo de recomendaciones
├── data/
│   ├── processed/peliculas_limpias.csv
│   └── raw/peliculas.csv
├── docs/
│   ├── informe_avance.pdf
│   └── user_flow_streaming.pdf
├── src/
│   ├── core/
│   │   ├── Normalizador.h
│   │   ├── Repository.h        # Contenedor genérico (Programación Genérica)
│   │   ├── Trie.h
│   │   └── Trie.cpp
│   ├── data/
│   │   ├── CSVReader.h
│   │   ├── CSVReader.cpp       # Carga paralela con std::async
│   │   ├── Pelicula.h
│   │   ├── PeliculaFactory.h   # Patrón Factory
│   │   ├── TagIndex.h
│   │   └── TagIndex.cpp
│   ├── search/
│   │   ├── Buscador.h
│   │   ├── Buscador.cpp
│   │   ├── RelevanceScorer.h   # Utilidad de tokenización
│   │   ├── IScoringStrategy.h  # Patrón Strategy (interfaz)
│   │   ├── PesosFijosStrategy.h    # Strategy: pesos fijos
│   │   └── TFIDFScoringStrategy.h  # Strategy: TF-IDF (activa por defecto)
│   ├── ui/
│   │   ├── IObserver.h         # Patrón Observer (interfaz)
│   │   ├── MenuBase.h
│   │   ├── MainMenu.h / .cpp   # Observer de UserData
│   │   ├── SearchMenu.h        # Búsqueda por texto
│   │   ├── TagSearchMenu.h     # Búsqueda por tag
│   │   ├── DetailMenu.h
│   │   ├── WatchLaterMenu.h
│   │   ├── RecommendMenu.h
│   │   ├── LikesMenu.h         # Ver mis likes
│   │   ├── Paginador.h
│   │   └── Paginador.cpp
│   └── user/
│       ├── UserData.h / .cpp   # Patrón Singleton + Observer (Subject)
│       ├── Recommender.h
│       └── Recommender.cpp
└── user_data/                  # Generado en runtime
    ├── likes.txt
    └── ver_mas_tarde.txt
```

## Referencias Bibliográficas
Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to Algorithms (3rd ed.). MIT Press. [Libro de referencia para estructuras de datos y complejidad temporal].

Stroustrup, B. (2013). The C++ Programming Language (4th ed.). Addison-Wesley. [Libro de referencia para el estándar C++ y programación genérica].

Gusfield, D. (1997). Algorithms on Strings, Trees, and Sequences. Cambridge University Press. [Referencia para Suffix Trees y Suffix Tries].

Gamma, E., Helm, R., Johnson, R., & Vlissides, J. (1994). Design Patterns: Elements of Reusable Object-Oriented Software. Addison-Wesley. [Libro de patrones de diseño (GoF)].

## Integrantes
Kiara Rojas M. – Interfaz de Usuario, Orquestación, Paginación, Patrones Factory y Observer, Documentación.

Nicole Ortega H. – Estructuras de Datos (Trie, Tags, Ranking), Patrón Strategy.

Thiago Frías P. – Persistencia, Recomendaciones, Programación Paralela, Patrón Singleton.