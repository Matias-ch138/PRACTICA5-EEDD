#ifndef THASHARBOL_H
#define THASHARBOL_H

#include <vector>
#include <list>
#include <iostream>
#include <cmath>
#include "Arbol.h"
using namespace std;

class ThashArbol {
private:
    enum Estado { VACIA, OCUPADA, BORRADA };

    struct Entrada {
        unsigned long clave;
        Arbol* dato;
        Estado estado;

        Entrada() : clave(0), dato(nullptr), estado(VACIA) {}
    };

    vector<Entrada> tabla_;
    list<Arbol> arboles_;
    unsigned int numElementos_;
    unsigned int maxColisiones_;
    unsigned int numMax10_;       
    unsigned long totalColisiones_;
    unsigned int numInserciones_;
    int tipoHash_;                
    unsigned int numRedispersiones_;
    float lambda_;

    bool esPrimo(unsigned int n) const;
    unsigned int siguientePrimo(unsigned int n) const;
    bool insertarEnTabla(unsigned long clave, Arbol* tree);

    unsigned int hashCuadratica(unsigned long clave, int intento) const;
    unsigned int hashDoble1(unsigned long clave, int intento) const;
    unsigned int hashDoble2(unsigned long clave, int intento) const;

    unsigned int hash(unsigned long clave, int intento) const;

public:
    // Constructores, operador asignación y destructor
    ThashArbol(int maxElementos, float lambda = 0.7f, int tipoHash = 1);
    ThashArbol(ThashArbol& thash);
    ThashArbol& operator=(ThashArbol& thash);
    ~ThashArbol();

    // Operaciones principales
    bool insertar(unsigned long clave, Arbol& tree);
    Arbol* buscar(unsigned long clave);
    bool borrar(unsigned long clave);
    unsigned int numElementos() const;

    // Métricas
    unsigned int maxColisiones() const;
    unsigned int numMax10() const;
    float promedioColisiones() const;
    float factorCarga() const;
    unsigned int tamTabla() const;
    void mostrarEstadoTabla() const;
    void setTipoHash(int tipo);
    unsigned int numRedispersiones() const;

    void redispersar(unsigned int tam);
};

#endif /* THASHARBOL_H */