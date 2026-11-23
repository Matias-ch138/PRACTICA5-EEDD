#include "LectorCSV.h"

#include <fstream>
#include <iostream>
#include "Especie.h"

/**
 * @brief Carga el fichero CSV indicado, mostrando cada especie por pantalla.
 *
 * Cada línea se analiza y, si tiene el número de campos esperado, se
 * construye una Especie que se almacena en el VDinamico
 * Las filas con un número de campos
 * distinto del esperado se descartan (ver filasDescartadas()).
 *
 * @param rutaFichero Ruta del fichero CSV a leer.
 * @return true si el fichero se ha podido abrir, false en caso contrario.
 */
bool LectorCSV::cargar(vector<Especie> &vector, const std::string& rutaFichero) {
    filasDescartadas_ = 0;

    std::ifstream fichero(rutaFichero);
    if (!fichero.is_open()) {
        return false;
    }

    std::string linea;
    if (std::getline(fichero, linea)) {
        eliminarBOM(linea);
    }

    //Declaradas fuera del bucle por eficiencia
    string codigo, nombreComun, nombreCientifico, tipoPlanta;
    while (std::getline(fichero, linea)) {
        if (linea.empty()) {
            continue;
        }

        if (contarCampos(linea) != 4) {
            ++filasDescartadas_;
            continue;
        }

        codigo = extraerCampo(linea, 0);
        nombreComun = extraerCampo(linea, 1);
        nombreCientifico = extraerCampo(linea, 2);
        tipoPlanta = extraerCampo(linea, 3);
        vector.push_back(Especie(codigo, nombreComun, nombreCientifico,tipoPlanta));

    }
    numeroEspecies_= vector.size();
    return true;
}

/**
 * @brief Número de especies leídas correctamente.
 * @return Cantidad de especies mostradas durante la última llamada a cargar().
 */
std::size_t LectorCSV::numeroEspecies() const {
    return numeroEspecies_;
}

/**
 * @brief Número de filas descartadas por no tener el número de campos esperado.
 * @return Cantidad de filas descartadas durante la última llamada a cargar().
 */
std::size_t LectorCSV::filasDescartadas() const {
    return filasDescartadas_;
}

/**
 * @brief Cuenta los campos de una línea CSV sin almacenarlos.
 *
 * Tiene en cuenta comillas envolventes y comillas escapadas ("") dentro
 * de un campo entrecomillado, tal como especifica RFC4180.
 *
 * @param linea Línea física a analizar.
 * @return Número de campos que contiene la línea.
 */
std::size_t LectorCSV::contarCampos(const std::string& linea) {
    std::size_t numCampos = 1;
    bool dentroComillas = false;
    const std::size_t n = linea.size();

    for (std::size_t i = 0; i < n; ) {
        const char c = linea[i];

        if (dentroComillas) {
            if (c == '"') {
                if (i + 1 < n && linea[i + 1] == '"') {
                    i += 2;
                } else {
                    dentroComillas = false;
                    ++i;
                }
            } else {
                ++i;
            }
        } else if (c == '"') {
            dentroComillas = true;
            ++i;
        } else if (c == ',') {
            ++numCampos;
            ++i;
        } else {
            ++i;
        }
    }

    return numCampos;
}

/**
 * @brief Extrae un único campo (por índice) de una línea CSV.
 *
 * Recorre la línea completa pero solo acumula el contenido del campo
 * solicitado; el resto de campos se descarta sin llegar a almacenarse.
 * Tiene en cuenta comillas envolventes y comillas escapadas ("").
 *
 * @param linea Línea física a analizar.
 * @param indiceObjetivo Índice (base 0) del campo a extraer.
 * @return Contenido del campo solicitado.
 */
std::string LectorCSV::extraerCampo(const std::string& linea, std::size_t indiceObjetivo) {
    std::string campo;
    std::size_t indiceActual = 0;
    bool dentroComillas = false;
    const std::size_t n = linea.size();

    for (std::size_t i = 0; i < n; ) {
        const char c = linea[i];

        if (dentroComillas) {
            if (c == '"') {
                if (i + 1 < n && linea[i + 1] == '"') {
                    if (indiceActual == indiceObjetivo) {
                        campo += '"';
                    }
                    i += 2;
                } else {
                    dentroComillas = false;
                    ++i;
                }
            } else {
                if (indiceActual == indiceObjetivo) {
                    campo += c;
                }
                ++i;
            }
        } else if (c == '"') {
            dentroComillas = true;
            ++i;
        } else if (c == ',') {
            if (indiceActual == indiceObjetivo) {
                return campo;
            }
            ++indiceActual;
            ++i;
        } else {
            if (indiceActual == indiceObjetivo) {
                campo += c;
            }
            ++i;
        }
    }

    return campo;
}

/**
 * @brief Elimina el BOM UTF-8 (0xEF 0xBB 0xBF) al inicio de la línea, si existe.
 * @param linea Línea a modificar in-place.
 */
void LectorCSV::eliminarBOM(std::string& linea) {
    static const char bom[3] = { static_cast<char>(0xEF), static_cast<char>(0xBB), static_cast<char>(0xBF) };
    if (linea.size() >= 3 && linea[0] == bom[0] && linea[1] == bom[1] && linea[2] == bom[2]) {
        linea.erase(0, 3);
    }
}

/**
 * @brief Muestra por pantalla los datos de una especie.
 * @param especie Especie a mostrar.
 */
void LectorCSV::mostrarEspecie(const Especie& especie) {
    std::cout << " - [" << especie.getcodigo_especie() << "] " << especie.getnombre_comun()
            << " (" << especie.getnombre_cientifico() << ") - "
            << especie.gettipo_planta() << "\n";
}
#include <algorithm> // Para std::replace



bool LectorCSV::cargarArboles(std::vector<Arbol>& arboles, const std::string& rutaFichero) {
    std::ifstream fichero(rutaFichero);
    if (!fichero.is_open()) {
        return false;
    }

    std::string linea;
    if (std::getline(fichero, linea)) {
        eliminarBOM(linea); // Limpiamos el BOM si existe en la cabecera
    }

    while (std::getline(fichero, linea)) {
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }
        if (linea.empty()) continue;

        std::string campos[12];
        std::size_t posicion = 0;
        
        // Extraemos los 12 campos separados por ';'
        for (int i = 0; i < 12; ++i) {
            std::size_t found = linea.find(';', posicion);
            if (found != std::string::npos) {
                campos[i] = linea.substr(posicion, found - posicion);
                posicion = found + 1;
            } else {
                campos[i] = linea.substr(posicion);
                break;
            }
        }

        if (campos[0].empty()) continue;

        try {
            // Normalizamos las comas decimales a puntos
            std::string x_str = campos[6];
            std::string y_str = campos[7];
            std::replace(x_str.begin(), x_str.end(), ',', '.');
            std::replace(y_str.begin(), y_str.end(), ',', '.');
            
            std::string alt_str = campos[10];
            std::replace(alt_str.begin(), alt_str.end(), ',', '.');
            
            std::string diam_str = campos[11];
            std::replace(diam_str.begin(), diam_str.end(), ',', '.');

            // Creamos el objeto UTM con las coordenadas
            UTM coord(std::stod(x_str), std::stod(y_str));
            
            float perimetro = diam_str.empty() ? 0.0f : std::stof(diam_str);
            float altura = alt_str.empty() ? 0.0f : std::stof(alt_str);

            // Llamada exacta al constructor de 9 parámetros de tu Arbol
            Arbol arbol(campos[0], std::stoi(campos[1]), std::stoi(campos[2]), campos[3],
                        std::stoi(campos[4]), campos[5], coord, perimetro, altura);
                        
            arboles.push_back(arbol);
        } catch (const std::exception&) {
            continue; 
        }
    }
    return true;
}