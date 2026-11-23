#include "ThashArbol.h"

bool ThashArbol::esPrimo(unsigned int n) const {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (unsigned int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

unsigned int ThashArbol::siguientePrimo(unsigned int n) const {
    if (n <= 2) return 2;
    if (n % 2 == 0) n++;
    while (!esPrimo(n)) {
        n += 2;
    }
    return n;
}

unsigned int ThashArbol::hashCuadratica(unsigned long clave, int intento) const {
    unsigned int M = tabla_.size();
    unsigned int h1 = clave % M;
    unsigned long desplazamiento = (unsigned long)intento * (unsigned long)intento;
    return (h1 + desplazamiento) % M;
}

unsigned int ThashArbol::hashDoble1(unsigned long clave, int intento) const {
    unsigned int M = tabla_.size();
    unsigned int h1 = clave % M;
    // Constante basada en la Proporción Áurea (Hashing de Fibonacci de Knuth).
    // Separa de forma simétrica y uniforme las claves que son consecutivas.
    unsigned int h2 = 1 + ((clave * 2654435761) % (M - 1));
    return (h1 + (unsigned long)intento * h2) % M;
}

unsigned int ThashArbol::hashDoble2(unsigned long clave, int intento) const {
    unsigned int M = tabla_.size();
    unsigned long x = clave;
    x ^= (x >> 16);
    // Constante matemática (tipo MurmurHash3): actúa como una "batidora de bits" 
    // para esparcir por la tabla IDs que sean muy parecidos o secuenciales.
    x *= 0x45d9f3b;
    x ^= (x >> 16);
    unsigned int h1 = x % M;
    unsigned int h2 = 1 + ((clave / M) % (M - 1));
    return (h1 + (unsigned long)intento * h2) % M;
}

unsigned int ThashArbol::hash(unsigned long clave, int intento) const {
    switch (tipoHash_) {
        case 1: return hashCuadratica(clave, intento);
        case 2: return hashDoble1(clave, intento);
        case 3:
        default: return hashDoble2(clave, intento);
    }
}

ThashArbol::ThashArbol(int maxElementos, float lambda, int tipoHash)
    : numElementos_(0), maxColisiones_(0), numMax10_(0),
    totalColisiones_(0), numInserciones_(0), tipoHash_(tipoHash) {
    unsigned int tamBase = (unsigned int)ceil(maxElementos / lambda);
    unsigned int tamPrimo = siguientePrimo(tamBase);
    tabla_.resize(tamPrimo);
}

ThashArbol::ThashArbol(ThashArbol& thash)
    : tabla_(thash.tabla_), numElementos_(thash.numElementos_),
    maxColisiones_(thash.maxColisiones_), numMax10_(thash.numMax10_),
    totalColisiones_(thash.totalColisiones_), numInserciones_(thash.numInserciones_),
    tipoHash_(thash.tipoHash_) {}

ThashArbol& ThashArbol::operator=(ThashArbol& thash) {
    if (this != &thash) {
        tabla_ = thash.tabla_;
        numElementos_ = thash.numElementos_;
        maxColisiones_ = thash.maxColisiones_;
        numMax10_ = thash.numMax10_;
        totalColisiones_ = thash.totalColisiones_;
        numInserciones_ = thash.numInserciones_;
        tipoHash_ = thash.tipoHash_;
    }
    return *this;
}

ThashArbol::~ThashArbol() {}

bool ThashArbol::insertar(unsigned long clave, Arbol& tree) {
    int intento = 0;
    int primerBorrado = -1;
    unsigned int M = tabla_.size();

    while (intento < (int)M) {
        unsigned int pos = hash(clave, intento);

        if (tabla_[pos].estado == OCUPADA) {
            if (tabla_[pos].clave == clave) {
                return false; 
            }
        } else if (tabla_[pos].estado == BORRADA) {
            if (primerBorrado == -1) {
                primerBorrado = (int)pos;
            }
        } else { 
            unsigned int posInsercion = (primerBorrado != -1) ? (unsigned int)primerBorrado : pos;
            tabla_[posInsercion].clave = clave;
            tabla_[posInsercion].dato = tree;
            tabla_[posInsercion].estado = OCUPADA;

            numElementos_++;
            numInserciones_++;
            unsigned int colisiones = (unsigned int)intento;
            totalColisiones_ += colisiones;
            
            if (colisiones > maxColisiones_) {
                maxColisiones_ = colisiones;
            }
            if (colisiones > 10) {
                numMax10_++;
            }
            return true;
        }
        intento++;
    }
    return false;
}

Arbol* ThashArbol::buscar(unsigned long clave) {
    int intento = 0;
    unsigned int M = tabla_.size();

    while (intento < (int)M) {
        unsigned int pos = hash(clave, intento);

        if (tabla_[pos].estado == VACIA) {
            return nullptr; 
        }
        if (tabla_[pos].estado == OCUPADA && tabla_[pos].clave == clave) {
            return &(tabla_[pos].dato);
        }
        intento++;
    }
    return nullptr;
}

bool ThashArbol::borrar(unsigned long clave) {
    int intento = 0;
    unsigned int M = tabla_.size();

    while (intento < (int)M) {
        unsigned int pos = hash(clave, intento);

        if (tabla_[pos].estado == VACIA) {
            return false;
        }
        if (tabla_[pos].estado == OCUPADA && tabla_[pos].clave == clave) {
            tabla_[pos].estado = BORRADA;
            numElementos_--;
            return true;
        }
        intento++;
    }
    return false;
}

unsigned int ThashArbol::numElementos() const {
    return numElementos_;
}

unsigned int ThashArbol::maxColisiones() const {
    return maxColisiones_;
}

unsigned int ThashArbol::numMax10() const {
    return numMax10_;
}

float ThashArbol::promedioColisiones() const {
    if (numInserciones_ == 0) return 0.0f;
    return (float)totalColisiones_ / (float)numInserciones_;
}

float ThashArbol::factorCarga() const {
    if (tabla_.empty()) return 0.0f;
    return (float)numElementos_ / (float)tabla_.size();
}

unsigned int ThashArbol::tamTabla() const {
    return (unsigned int)tabla_.size();
}

void ThashArbol::mostrarEstadoTabla() const {
    cout << "=== ESTADO DE LA TABLA HASH ===" << endl;
    cout << "Tamano de la tabla: " << tamTabla() << endl;
    cout << "Elementos almacenados: " << numElementos() << endl;
    cout << "Factor de carga: " << factorCarga() << endl;
    cout << "Maximo de colisiones: " << maxColisiones() << endl;
    cout << "Inserciones con > 10 colisiones: " << numMax10() << endl;
    cout << "Promedio de colisiones: " << promedioColisiones() << endl;
    cout << "===============================" << endl;
}

void ThashArbol::setTipoHash(int tipo) {
    tipoHash_ = tipo;
}

void ThashArbol::redispersar(unsigned int tam) {
    unsigned int nuevoTam = siguientePrimo(tam);
    vector<Entrada> tablaAntigua = tabla_;

    tabla_.clear();
    tabla_.resize(nuevoTam);
    numElementos_ = 0;
    maxColisiones_ = 0;
    numMax10_ = 0;
    totalColisiones_ = 0;
    numInserciones_ = 0;

    for (size_t i = 0; i < tablaAntigua.size(); ++i) {
        if (tablaAntigua[i].estado == OCUPADA) {
            insertar(tablaAntigua[i].clave, tablaAntigua[i].dato);
        }
    }
}