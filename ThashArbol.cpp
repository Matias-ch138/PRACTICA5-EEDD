#include "ThashArbol.h"
#include <stdexcept>
#include <utility>

/**
 * @brief saber si un número es primo
 * 
 * @param n número que queremos comprobar que es primo
 * @return true @param n es primo
 * @return false @param n no es primo
 */
bool ThashArbol::esPrimo(unsigned int n) const {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (unsigned int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}
/**
 * @brief devuelve el siguiente número que sea primo más grande que @param n
 * 
 * @param n número del que partimos
 * @return unsigned int número primo más grande que @param n
 */
unsigned int ThashArbol::siguientePrimo(unsigned int n) const {
    if (n <= 2) return 2;
    if (n % 2 == 0) n++;
    while (!esPrimo(n)) {
        n += 2;
    }
    return n;
}
/**
 * @brief función hash cuadratica
 * 
 * @param clave clave del elemento que queremos meter
 * @param intento número de intentos (colisiones)
 * @return unsigned int hash cuadratico del elemento con @param clave
 */
unsigned int ThashArbol::hashCuadratica(unsigned long clave, int intento) const {
    unsigned int M = tabla_.size();
    unsigned int h1 = clave % M;
    unsigned long desplazamiento = (unsigned long)intento * (unsigned long)intento;
    return (h1 + desplazamiento) % M;
}
/**
 * @brief función hashDoble (Hashing de Fibonacci de Knuth)
 * 
 * @param clave clave del elemento que queremos meter
 * @param intento número de intentos (colisiones)
 * @return unsigned int hashDoble de la función de Hash de Knuth del elemento con @param clave
 */
unsigned int ThashArbol::hashDoble1(unsigned long clave, int intento) const {
    unsigned int M = tabla_.size();
    unsigned int h1 = clave % M;
    // Constante basada en la Proporción Áurea (Hashing de Fibonacci de Knuth).
    // Separa de forma simétrica y uniforme las claves que son consecutivas.
    unsigned int h2 = 1 + ((clave * 2654435761) % (M - 1));
    return (h1 + (unsigned long)intento * h2) % M;
}
/**
 * @brief función hashDoble tipo MurmurHash3
 * 
 * @param clave clave del elemento que queremos meter
 * @param intento número de intentos (colisiones)
 * @return unsigned int hashDoble tipo MurmurHas3 del elemento con @param clave
 */
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
/**
 * @brief función auxiliar para calcular el hash de un elemento con @param clave seleccionando diferentes funciones hash
 * 
 * @param clave clave del elemento que queremos meter
 * @param intento número de intentos (colisiones)
 * @return unsigned int hash que queremos calcular del elemento con @param clave
 */
unsigned int ThashArbol::hash(unsigned long clave, int intento) const {
    switch (tipoHash_) {
        case 1: return hashCuadratica(clave, intento);
        case 2: return hashDoble1(clave, intento);
        case 3:
        default: return hashDoble2(clave, intento);
    }
}

/**
 * @brief Constructor de una Tabla Hash de Arboles
 * 
 * @param maxElementos número de elementos que se van a meter
 * @param lambda factor de carga
 * @param tipoHash tipo de función hash que se va a utilizar
 */
ThashArbol::ThashArbol(int maxElementos, float lambda, int tipoHash)
    : numElementos_(0), maxColisiones_(0), numMax10_(0),
    totalColisiones_(0), numInserciones_(0), tipoHash_(tipoHash),
    numRedispersiones_(0), lambda_(lambda) {
    unsigned int tamBase = (unsigned int)ceil(maxElementos / lambda);
    unsigned int tamPrimo = siguientePrimo(tamBase);
    tabla_.resize(tamPrimo);
}
/**
 * @brief Constructor copia de una tabla Hash de Arboles
 * 
 * @param thash tabla Hash de Arboles que vamos a copiar
 */
ThashArbol::ThashArbol(ThashArbol& thash)
    : tabla_(thash.tabla_), numElementos_(thash.numElementos_),
    maxColisiones_(thash.maxColisiones_), numMax10_(thash.numMax10_),
    totalColisiones_(thash.totalColisiones_), numInserciones_(thash.numInserciones_),
    tipoHash_(thash.tipoHash_), numRedispersiones_(thash.numRedispersiones_),
    lambda_(thash.lambda_) {
    for (size_t i = 0; i < thash.tabla_.size(); ++i) {
        if (thash.tabla_[i].estado == OCUPADA) {
            arboles_.push_back(*thash.tabla_[i].dato);
            tabla_[i].dato = &arboles_.back();
        }
    }
}
/**
 * @brief operador de asignación
 * 
 * @param thash tabla Hash de Arboles que se va a asignar a otra.
 * @return ThashArbol& tabla Hash de Arboles que ha sido asignada
 */
ThashArbol& ThashArbol::operator=(ThashArbol& thash) {
    if (this != &thash) {
        ThashArbol copia(thash);
        tabla_.swap(copia.tabla_);
        arboles_.swap(copia.arboles_);
        std::swap(numElementos_, copia.numElementos_);
        std::swap(maxColisiones_, copia.maxColisiones_);
        std::swap(numMax10_, copia.numMax10_);
        std::swap(totalColisiones_, copia.totalColisiones_);
        std::swap(numInserciones_, copia.numInserciones_);
        std::swap(tipoHash_, copia.tipoHash_);
        std::swap(numRedispersiones_, copia.numRedispersiones_);
        std::swap(lambda_, copia.lambda_);
    }
    return *this;
}
/**
 * @brief Destructor de una Tabla Hash de Arboles
 * 
 */
ThashArbol::~ThashArbol() {}
/**
 * @brief Método de insercción de una Tabla Hash de Arboles
 * 
 * @param clave clave del elemento que queremos insertar
 * @param tree elemento que queremos insertar
 * @return true si se ha insertado
 * @return false no se ha podido insertar
 */
bool ThashArbol::insertar(unsigned long clave, Arbol& tree) {
    arboles_.push_back(tree);
    Arbol* arbolAlmacenado = &arboles_.back();

    if (!insertarEnTabla(clave, arbolAlmacenado)) {
        arboles_.pop_back();
        return false;
    }

    if (factorCarga() > lambda_) {
        redispersar(static_cast<unsigned int>(ceil(tabla_.size() * 1.3)));
    }
    return true;
}

bool ThashArbol::insertarEnTabla(unsigned long clave, Arbol* tree) {
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
/**
 * @brief busca un elemento guardado en una Tabla Hash de Arboles
 * 
 * @param clave clave del elemento
 * @return Arbol* arbol cuya clave es @param clave
 */
Arbol* ThashArbol::buscar(unsigned long clave) {
    int intento = 0;
    unsigned int M = tabla_.size();

    while (intento < (int)M) {
        unsigned int pos = hash(clave, intento);

        if (tabla_[pos].estado == VACIA) {
            return nullptr; 
        }
        if (tabla_[pos].estado == OCUPADA && tabla_[pos].clave == clave) {
            return tabla_[pos].dato;
        }
        intento++;
    }
    return nullptr;
}
/**
 * @brief borra un elemento cuya clavve es @param clave
 * 
 * @param clave clave del elemento que queremos borrar
 * @return true si se ha borrado correctamente
 * @return false si no se ha podido borrar
 */
bool ThashArbol::borrar(unsigned long clave) {
    int intento = 0;
    unsigned int M = tabla_.size();

    while (intento < (int)M) {
        unsigned int pos = hash(clave, intento);

        if (tabla_[pos].estado == VACIA) {
            return false;
        }
        if (tabla_[pos].estado == OCUPADA && tabla_[pos].clave == clave) {
            Arbol* arbolBorrado = tabla_[pos].dato;
            tabla_[pos].estado = BORRADA;
            tabla_[pos].dato = nullptr;
            numElementos_--;

            list<Arbol>::iterator it = arboles_.begin();
            while (it != arboles_.end()) {
                if (&*it == arbolBorrado) {
                    arboles_.erase(it);
                    break;
                }
                ++it;
            }
            return true;
        }
        intento++;
    }
    return false;
}
/**
 * @brief devuelve el número de elementos que hay en la Tabla Hash de Arboles
 * 
 * @return unsigned int número de elementos
 */
unsigned int ThashArbol::numElementos() const {
    return numElementos_;
}
/**
 * @brief devuelve las máx colisiones que se han producido al insertar un elemento
 * 
 * @return unsigned int el número máximo de colisiones que se han producido
 */
unsigned int ThashArbol::maxColisiones() const {
    return maxColisiones_;
}
/**
 * @brief devuelve el número de veces que se han insertado elementos con mas de 10 colisiones
 * 
 * @return unsigned int número de veces que se han insertado elementos con mas de 10 colisiones
 */
unsigned int ThashArbol::numMax10() const {
    return numMax10_;
}
/**
 * @brief devuelve el promedio de colisiones al insertar elementos
 * 
 * @return float promedio de colisiones al insertar elementos
 */
float ThashArbol::promedioColisiones() const {
    if (numInserciones_ == 0) return 0.0f;
    return (float)totalColisiones_ / (float)numInserciones_;
}
/**
 * @brief devuelve el factor de carga de la table
 * 
 * @return float factor de carga de la tabla
 */
float ThashArbol::factorCarga() const {
    if (tabla_.empty()) return 0.0f;
    return (float)numElementos_ / (float)tabla_.size();
}
/**
 * @brief devuelve el tamaño de la tabla
 * 
 * @return unsigned int tamaño de la tabla
 */
unsigned int ThashArbol::tamTabla() const {
    return (unsigned int)tabla_.size();
}
/**
 * @brief muestra el estado de la tabla por pantalla
 * 
 */
void ThashArbol::mostrarEstadoTabla() const {
    cout << "=== ESTADO DE LA TABLA HASH ===" << endl;
    cout << "Tamano de la tabla: " << tamTabla() << endl;
    cout << "Elementos almacenados: " << numElementos() << endl;
    cout << "Factor de carga: " << factorCarga() << endl;
    cout << "Maximo de colisiones: " << maxColisiones() << endl;
    cout << "Inserciones con > 10 colisiones: " << numMax10() << endl;
    cout << "Promedio de colisiones: " << promedioColisiones() << endl;
    cout << "Redispersiones realizadas: " << numRedispersiones() << endl;
    cout << "=================================" << endl;
}
/**
 * @brief establecer el tipo de función de hash que vamos a utilizar en la tabla
 * 
 * @param tipo tipo de función hash
 */
void ThashArbol::setTipoHash(int tipo) {
    tipoHash_ = tipo;
}
unsigned int ThashArbol::numRedispersiones() const {
    return numRedispersiones_;
}
/**
 * @brief se redispersa la tabla hash con un nuevo tamaño @param tam
 * 
 * @param tam tamaño nuevo con el que se va a redispersar la tabla
 */
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
            if (!insertarEnTabla(tablaAntigua[i].clave, tablaAntigua[i].dato)) {
                throw logic_error("No se pudo reinsertar un árbol durante la redispersión.");
            }
        }
    }
    ++numRedispersiones_;
}