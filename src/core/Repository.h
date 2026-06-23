#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <unordered_map>
#include <vector>
#include <stdexcept>  // Para std::out_of_range

// ============================================================
// CLASE GENERICA: Repository<T>
// ============================================================
// T: cualquier tipo de dato que tenga un campo "int id".
// ============================================================
template <typename T>
class Repository {
private:
    // Almacenamos los datos en un mapa: ID -> Objeto
    std::unordered_map<int, T> items;

public:
    // -------- Agregar un elemento --------
    void add(const T& item) {
        // 'item' debe tener un campo público 'id'.
        // Asumimos que el ID ya está asignado (ej: desde CSVReader).
        items[item.id] = item;
    }

    // -------- Obtener por ID (con verificacion) --------
    T& getById(int id) {
        auto it = items.find(id);
        if (it == items.end()) {
            throw std::out_of_range("Repository: ID no encontrado: " + std::to_string(id));
        }
        return it->second; // Devuelve la referencia al objeto
    }

    // Version const (para cuando no queremos modificar)
    const T& getById(int id) const {
        auto it = items.find(id);
        if (it == items.end()) {
            throw std::out_of_range("Repository: ID no encontrado: " + std::to_string(id));
        }
        return it->second;
    }

    // -------- Obtener TODOS los elementos (como vector) --------
    std::vector<T> getAll() const {
        std::vector<T> vec;
        vec.reserve(items.size()); // Optimizacion: reservamos espacio
        for (const auto& pair : items) {
            vec.push_back(pair.second);
        }
        return vec;
    }

    // -------- Eliminar un elemento --------
    bool remove(int id) {
        return items.erase(id) > 0; // erase devuelve cuantos borro (1 o 0)
    }

    // -------- Verificar si existe --------
    bool exists(int id) const {
        return items.find(id) != items.end();
    }

    // -------- Obtener la cantidad de elementos --------
    size_t size() const {
        return items.size();
    }

    // -------- Limpiar todo --------
    void clear() {
        items.clear();
    }
};

#endif