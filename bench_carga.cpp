// Benchmark: compara la carga secuencial vs. paralela del CSV.
// Compilar:  g++ -std=c++20 -O2 -Isrc/... bench_carga.cpp src/data/CSVReader.cpp src/data/TagIndex.cpp -o bench
#include "src/data/CSVReader.h"
#include "src/data/TagIndex.h"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
    const std::string ruta = "data/processed/peliculas_limpias.csv";
    const int repeticiones = 5;

    std::cout << "Nucleos disponibles: " << std::thread::hardware_concurrency() << "\n";

    double sumSeq = 0, sumPar = 0;
    size_t nSeq = 0, nPar = 0;
    for (int r = 0; r < repeticiones; ++r) {
        {
            TagIndex idx;
            auto t0 = std::chrono::high_resolution_clock::now();
            auto v = CSVReader::cargarDatos(ruta, idx);
            auto t1 = std::chrono::high_resolution_clock::now();
            sumSeq += std::chrono::duration<double, std::milli>(t1 - t0).count();
            nSeq = v.size();
        }
        {
            TagIndex idx;
            auto t0 = std::chrono::high_resolution_clock::now();
            auto v = CSVReader::cargarDatosParalelo(ruta, idx);
            auto t1 = std::chrono::high_resolution_clock::now();
            sumPar += std::chrono::duration<double, std::milli>(t1 - t0).count();
            nPar = v.size();
        }
    }

    double seq = sumSeq / repeticiones;
    double par = sumPar / repeticiones;
    std::cout << "Peliculas: secuencial=" << nSeq << "  paralelo=" << nPar << "\n";
    std::cout << "Secuencial: " << seq << " ms (promedio de " << repeticiones << ")\n";
    std::cout << "Paralelo:   " << par << " ms (promedio de " << repeticiones << ")\n";
    std::cout << "Speedup:    " << (seq / par) << "x\n";
    return 0;
}
