#ifndef IOBSERVER_H
#define IOBSERVER_H

// Interfaz para el patrón Observer.
// Cualquier clase que quiera ser notificada de cambios debe implementar esto.
class IObserver {
public:
    // Este método se llama cuando el Subject (UserData) cambia.
    virtual void onDataChanged() = 0;
    
    // Destructor virtual (buena práctica)
    virtual ~IObserver() = default;
};

#endif