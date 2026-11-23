#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include <string>
#include "Arbol.h"
#include "ThashArbol.h"
#include "LectorCSV.h"
#include "Especie.h"
#include "Parque.h"

using namespace std;
using namespace std::chrono;

int main() {
    cout << "=== INICIANDO PROGRAMA DE PRUEBA 1 ===" << endl;

    // 1. Cargar todos los árboles usando tu LectorCSV
    vector<Arbol> todosLosArboles;
    LectorCSV lector;

    if (!lector.cargarArboles(todosLosArboles, "./data/arboladoMadrid.csv")) {
        cout << "Error: No se pudo abrir el archivo arboladoMadrid.csv" << endl;
        return 1;
    }

    int totalArboles = (int)todosLosArboles.size();
    cout << "Arboles cargados correctamente: " << totalArboles << endl << endl;

    // =========================================================================
    // FASE 1: PROBAR LAS 6 CONFIGURACIONES
    // =========================================================================
    float lambdas[] = {0.65f, 0.68f};
    int tiposHash[] = {1, 2, 3}; // 1: Cuadratica, 2: Doble1, 3: Doble2

    cout << "--- FASE 1: BUSCANDO LA MEJOR CONFIGURACION ---" << endl;
    for (float lambda : lambdas) {
        for (int tipo : tiposHash) {
            ThashArbol tabla(totalArboles, lambda, tipo);
            
            for (size_t i = 0; i < todosLosArboles.size(); ++i) {
                try {
                    unsigned long clave = stoul(todosLosArboles[i].get_assetnum());
                    tabla.insertar(clave, todosLosArboles[i]);
                } catch (const exception&) {
                    continue;
                }
            }

            cout << "Lambda: " << lambda << " | Hash: " << tipo 
                << " | Tamano Primo: " << tabla.tamTabla() 
                << " | Max Colisiones: " << tabla.maxColisiones()
                << " | Promedio: " << tabla.promedioColisiones() 
                << " | >10 Colisiones: " << tabla.numMax10() << endl;
        }
    }
    cout << endl;

    // =========================================================================
    // FASE 2: LA CARRERA (BENCHMARK) CON 2000 ARBOLES
    // =========================================================================
    cout << "--- FASE 2: TABLA HASH vs STD::LIST ---" << endl;
    
    // a) Elegir 2000 claves al azar
    vector<unsigned long> clavesHashAleatorias;
    vector<string> clavesListaAleatorias;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, totalArboles - 1);

    for (int i = 0; i < 2000; i++) {
        int indiceAleatorio = dis(gen);
        string assetNum = todosLosArboles[indiceAleatorio].get_assetnum();
        try {
            clavesHashAleatorias.push_back(stoul(assetNum));
            clavesListaAleatorias.push_back(assetNum);
        } catch (const exception&) {
            i--; // Reintentar si algún assetnum no es numérico puro
        }
    }

    // b) Preparar la Tabla Hash (Ajusta 0.68f y 2 según la ganadora de la Fase 1)
    ThashArbol tablaGanadora(totalArboles, 0.68f, 2);
    list<Arbol> listaArboles;

    for (size_t i = 0; i < todosLosArboles.size(); ++i) {
        try {
            unsigned long clave = stoul(todosLosArboles[i].get_assetnum());
            tablaGanadora.insertar(clave, todosLosArboles[i]);
            listaArboles.push_back(todosLosArboles[i]);
        } catch (const exception&) {
            continue;
        }
    }

    // c) Cronometrar la Tabla Hash
    auto inicioHash = high_resolution_clock::now();
    int encontradosHash = 0;
    for (unsigned long clave : clavesHashAleatorias) {
        if (tablaGanadora.buscar(clave) != nullptr) {
            encontradosHash++;
        }
    }
    auto finHash = high_resolution_clock::now();
    auto tiempoHash = duration_cast<microseconds>(finHash - inicioHash).count();

    // d) Cronometrar la Lista (Búsqueda secuencial)
    auto inicioLista = high_resolution_clock::now();
    int encontradosLista = 0;
    for (const string& claveStr : clavesListaAleatorias) {
        for (auto it = listaArboles.begin(); it != listaArboles.end(); ++it) {
            if (it->get_assetnum() == claveStr) {
                encontradosLista++;
                break;
            }
        }
    }
    auto finLista = high_resolution_clock::now();
    auto tiempoLista = duration_cast<microseconds>(finLista - inicioLista).count();

    // Mostrar resultados
    cout << "Buscando 2000 arboles aleatorios..." << endl;
    cout << "Tiempo Tabla Hash : " << tiempoHash << " microsegundos (Encontrados: " << encontradosHash << ")" << endl;
    cout << "Tiempo std::list  : " << tiempoLista << " microsegundos (Encontrados: " << encontradosLista << ")" << endl;

    return 0;
}