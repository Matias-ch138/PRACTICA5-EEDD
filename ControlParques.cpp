#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "ControlParques.h"
#include "LectorCSV.h"

using namespace std;
static bool leerCampoCSV(const string& linea, size_t& posicion, string& campo, char separador = ',') {
    campo.clear();
    bool entreComillas = posicion < linea.size() && linea[posicion] == '"';
    if (entreComillas) {
        ++posicion;
    }

    while (posicion < linea.size()) {
        char caracter = linea[posicion];
        if (entreComillas && caracter == '"') {
            if (posicion + 1 < linea.size() && linea[posicion + 1] == '"') {
                campo += '"';
                posicion += 2;
            } else {
                entreComillas = false;
                ++posicion;
            }
        } else if (!entreComillas && caracter == separador) {
            ++posicion;
            return true;
        } else {
            campo += caracter;
            ++posicion;
        }
    }

    return !entreComillas;
}

static bool leerRegistroCSV(const string& linea, string campos[], size_t numeroCampos, char separador = ';') {
    size_t posicion = 0;
    for (size_t i = 0; i < numeroCampos; ++i) {
        if (!leerCampoCSV(linea, posicion, campos[i], separador)) {
            return false;
        }
        if (i + 1 < numeroCampos && posicion == linea.size() && linea.back() != separador) {
            return false;
        }
    }
    return true;
}

static float leerMedidaCSV(const string& campo) {
    if (campo.empty()) {
        return 0.0f;
    }
    string normalizado = campo;
    replace(normalizado.begin(), normalizado.end(), ',', '.');
    return stof(normalizado);
}

static void limpiarLinea(string& linea) {
    if (!linea.empty() && linea.back() == '\r') {
        linea.pop_back();
    }
    // Eliminar BOM UTF-8 si existe
    if (linea.size() >= 3 &&
        static_cast<unsigned char>(linea[0]) == 0xEF &&
        static_cast<unsigned char>(linea[1]) == 0xBB &&
        static_cast<unsigned char>(linea[2]) == 0xBF) {
        linea.erase(0, 3);
    }
}
unsigned long ControlParques::djb2(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = static_cast<unsigned char>(*str++))) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}
static Especie* buscarEspecie(vector<Especie>& especies, const string& codigo) {
    int inicio = 0;
    int fin = static_cast<int>(especies.size()) - 1;
    while (inicio <= fin) {
        int medio = inicio + (fin - inicio) / 2;
        const string& codigoMedio = especies[medio].get_codigo_especie();
        if (codigoMedio == codigo) {
            return &especies[medio];
        }
        if (codigoMedio < codigo) {
            inicio = medio + 1;
        } else {
            fin = medio - 1;
        }
    }
    return nullptr;
}

static Parque* buscarParquePorCodigo(list<Parque>& parques, int codigo) {
    list<Parque>::iterator it=parques.begin();
    while(it!=parques.end()){
        if(it->get_cod_parque()==codigo) return &*it;
        it++;
    }
    return nullptr;
}
ControlParques::ControlParques() : trees(135000, 0.65f, 2) {
}
ControlParques::ControlParques(const string& rutaEspecies, const string& rutaParques,const string& rutaArboles, const string& rutaCajasNido)
    : trees(135000, 0.65f, 2) {

    // 1. Cargar Especies directamente en map<string, Especie>
    vector<Especie> vecEspeciesAux;
    LectorCSV lector;
    if (lector.cargar(vecEspeciesAux, rutaEspecies)) {
        for (size_t i = 0; i < vecEspeciesAux.size(); ++i) {
            varieties.insert({vecEspeciesAux[i].get_codigo_especie(), vecEspeciesAux[i]});
        }
    } else {
        cerr << "Error: No se pudo abrir el archivo de especies: " << rutaEspecies << endl;
    }

    // 2. Cargar Parques directamente en map<int, Parque>
    ifstream ficheroParques(rutaParques);
    if (ficheroParques.is_open()) {
        string linea;
        while (getline(ficheroParques, linea)) {
            limpiarLinea(linea);
            if (linea.empty()) continue;

            string cod_str, nombre_parque;
            size_t posicion = 0;
            if (leerCampoCSV(linea, posicion, cod_str) && leerCampoCSV(linea, posicion, nombre_parque)) {
                try {
                    int cod = stoi(cod_str);
                    parks.insert({cod, Parque(cod, nombre_parque)});
                } catch (const exception&) {
                    continue;
                }
            }
        }
        ficheroParques.close();
    } else {
        cerr << "Error: No se pudo abrir el archivo de parques: " << rutaParques << endl;
    }

    // 3. Cargar Árboles en ThashArbol y guardar assetnums en vector auxiliar
    vector<string> assetnumsAux;
    assetnumsAux.reserve(135000);

    ifstream ficheroArboles(rutaArboles);
    if (!ficheroArboles.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de arboles: " << rutaArboles << endl;
        return;
    }

    string lineaArbol;
    getline(ficheroArboles, lineaArbol); // Saltar cabecera
    while (getline(ficheroArboles, lineaArbol)) {
        limpiarLinea(lineaArbol);
        if (lineaArbol.empty()) continue;

        string campos[12];
        if (!leerRegistroCSV(lineaArbol, campos, 12, ';') || campos[0].empty()) {
            continue;
        }

        try {
            string x = campos[6];
            string y = campos[7];
            replace(x.begin(), x.end(), ',', '.');
            replace(y.begin(), y.end(), ',', '.');
            UTM coord(stod(x), stod(y));

            Arbol arbol(campos[0], stoi(campos[1]), stoi(campos[2]), campos[3],
                        stoi(campos[4]), campos[5], coord,
                        leerMedidaCSV(campos[10]), leerMedidaCSV(campos[11]));

            // Buscar especie en map<string, Especie>
            Especie* especieEncontrada = nullptr;
            map<string, Especie>::iterator itEsp = varieties.find(campos[8]);
            if (itEsp != varieties.end()) {
                especieEncontrada = &(itEsp->second);
            }
            arbol.setEspecie(especieEncontrada);

            // Insertar en ThashArbol usando clave djb2
            unsigned long claveHash = djb2(arbol.get_assetnum().c_str());
            if (!trees.insertar(claveHash, arbol)) {
                continue;
            }

            assetnumsAux.push_back(arbol.get_assetnum());

            Arbol* arbolAlmacenado = trees.buscar(claveHash);
            if (arbolAlmacenado != nullptr) {
                map<int, Parque>::iterator itParque = parks.find(arbolAlmacenado->get_num_parque());
                if (itParque != parks.end()) {
                    itParque->second.insertarArbol(arbolAlmacenado);
                    arbolAlmacenado->setParque(&(itParque->second));
                }

                if (especieEncontrada != nullptr) {
                    treeName.insert({especieEncontrada->get_nombre_comun(), arbolAlmacenado});
                }
            }
        } catch (const exception&) {
            continue;
        }
    }
    ficheroArboles.close();

    // 4. Cargar Cajas Nido desde cajas-nido.csv
    ifstream ficheroCajas(rutaCajasNido);
    if (!ficheroCajas.is_open()) {
        cerr << "Error: No se pudo abrir el archivo de cajas nido: " << rutaCajasNido << endl;
        return;
    }

    string lineaCaja;
    getline(ficheroCajas, lineaCaja); // Saltar cabecera
    while (getline(ficheroCajas, lineaCaja)) {
        limpiarLinea(lineaCaja);
        if (lineaCaja.empty()) continue;

        // Nota: Ajustar los índices de columna si tu CSV tiene otro orden
        // Formato habitual: distrito;barrio;ubicacion;utm_x;utm_y
        string camposCaja[5];
        if (!leerRegistroCSV(lineaCaja, camposCaja, 5, ';')) {
            continue;
        }

        try {
            string xStr = camposCaja[3];
            string yStr = camposCaja[4];
            replace(xStr.begin(), xStr.end(), ',', '.');
            replace(yStr.begin(), yStr.end(), ',', '.');
            UTM coordCaja(stod(xStr), stod(yStr));

            CajaNido caja(camposCaja[0], camposCaja[1], camposCaja[2], coordCaja);
            nests.push_back(caja);
        } catch (const exception&) {
            continue;
        }
    }
    ficheroCajas.close();

    // 5. Asociar cada Caja Nido al árbol más cercano con distancia^2 < 100 (< 10m)
    unsigned int cajasAsociadas = 0;
    for (size_t i = 0; i < nests.size(); ++i) {
        Arbol* arbolMasCercano = nullptr;
        double minDist2 = 100.0; // Umbral estricto < 100

        for (size_t j = 0; j < assetnumsAux.size(); ++j) {
            Arbol* arbolActual = buscarArbol(assetnumsAux[j]);
            if (arbolActual != nullptr) {
                double d2 = nests[i].getCoord().distancia2(arbolActual->get_coord());
                if (d2 < minDist2) {
                    minDist2 = d2;
                    arbolMasCercano = arbolActual;
                }
            }
        }

        nests[i].setArbol(arbolMasCercano);
        if (arbolMasCercano != nullptr) {
            cajasAsociadas++;
        }
    }

    // 6. Mostrar resumen obligatorio de carga por pantalla
    cout << "--- RESUMEN DE CARGA CONTROLPARQUES ---" << endl;
    cout << "Total arboles cargados: " << trees.numElementos() << endl;
    cout << "Total cajas nido procesadas: " << nests.size() << endl;
    cout << "Cajas nido asociadas a un arbol: " << cajasAsociadas << endl;
    cout << "Cajas nido sin asociar: " << (nests.size() - cajasAsociadas) << endl;
    cout << "---------------------------------------" << endl;
}


int ControlParques::contarEspecieParque(string ncomun){
    int contador = 0;
    map<int,Parque>::iterator it=parks.begin();
    while(it!=parks.end()){
        if(it->second.existeNbreCom(ncomun)){
            contador++;
        }
        it++;
    }
    return contador;
}

vector<Especie> ControlParques::listadoEspecieSubNbre(string cadena){
    vector<Especie> resultados;
    map<string,Especie>::iterator it=varieties.begin();
    while(it!=varieties.end()){
        if(it->second.getnombre_cientifico().find(cadena) != string::npos){
            resultados.push_back(it->second);
        }
        it++;
    }
    return resultados;
}

Parque* ControlParques::buscarParque(const string& nbre_parque){
    map<int,Parque>::iterator it=parks.begin();
    while(it!=parks.end()){
        if(it->second.get_nbre_parque() == nbre_parque){
            return &it->second;
        }
    it++;  
    }
    return nullptr;
}


map<string,Especie>& ControlParques::get_varieties(){
    return varieties;
}

const map<string,Especie>& ControlParques::get_varieties() const{
    return varieties;
}

map<int,Parque>& ControlParques::get_parks(){
    return parks;
}

const map<int,Parque>& ControlParques::get_parks() const{
    return parks;  
}

vector<CajaNido>& ControlParques::get_nests() {
    return nests;
}

const vector<CajaNido>& ControlParques::get_nests() const {
    return nests;
}

map<int,Parque>& ControlParques::get_nbre_parque(){
    return parks;
}

const map<int,Parque>& ControlParques::get_nbre_parque() const{
    return parks;
}
unsigned int ControlParques::tamTabla() const {
    return trees.tamTabla();
}

void ControlParques::redispersar(unsigned int tam) {
    trees.redispersar(tam);
}

void ControlParques::mostrarEstadoTabla() const {
    trees.mostrarEstadoTabla();
}

Arbol* ControlParques::buscarArbol(string asNum){
    return trees.buscar(djb2(asNum.c_str()));
}
vector<Arbol*> ControlParques::listaArbParque(string nbre_parque){
    Parque* parque = buscarParque(nbre_parque);
    if (parque != nullptr) {
        return parque->get_contains();
    }
    return vector<Arbol*>();
}
Arbol* ControlParques::arbolMasAlto(){
    Arbol* arbolMasAlto=nullptr;
    map<int,Parque>::iterator it=parks.begin();
    while(it!=parks.end()){
        Arbol* mayorParque=it->second.arbolMasAlto();
        if(mayorParque != nullptr){
            if(arbolMasAlto==nullptr || mayorParque->get_altura()>arbolMasAlto->get_altura()){
                arbolMasAlto=mayorParque;
            }
        }
        it++;
    }
    return arbolMasAlto;
}
int ControlParques::contarEspecies(string nbre_parque){
    Parque* parque =buscarParque(nbre_parque);
    if(parque!=nullptr){
        return parque->contarEspecies();
    }
    return 0;
}
int ControlParques::contarTipos(){
    map<int,Parque>::iterator it=parks.begin();
    set<string> tiposUnicos;
    while(it!=parks.end()){
        const vector<Arbol*> &arboles= it->second.get_contains();
        for (int i = 0; i < arboles.size(); i++){
            string tipo = arboles[i]->getTipoEspecie();
            if (!tipo.empty()) {
                tiposUnicos.insert(tipo);
            }
        }
        it++;
    }
    return tiposUnicos.size();
}

unsigned int ControlParques::numArboles() const {
    return trees.numElementos();
}

vector<Parque*> ControlParques::listarParquesArbol(const string& ncomun){
    set<Parque*> parquesUnicos;
    pair<multimap<string, Arbol*>::iterator, multimap<string, Arbol*>::iterator> pareja = treeName.equal_range(ncomun); 
    while(pareja.first!=pareja.second){
        Parque* paux=pareja.first->second->getParque();
        if(paux!=nullptr){
            parquesUnicos.insert(paux);
        }
        
        pareja.first++;
    }
    return vector<Parque*>(parquesUnicos.begin(), parquesUnicos.end());
}

vector<Arbol*> ControlParques::buscarArbolNcomun(const string& ncomun){
    vector<Arbol*> resultados;
    pair<multimap<string, Arbol*>::iterator, multimap<string, Arbol*>::iterator> pareja = treeName.equal_range(ncomun); 
    while(pareja.first!=pareja.second){
        resultados.push_back(pareja.first->second);
        pareja.first++;
    }
    return resultados;
}

vector<Arbol*> ControlParques::buscarArbolNcomunComienzo(const string& ncomun){
    multimap<string,Arbol*>::iterator it = treeName.lower_bound(ncomun);
    vector<Arbol*> resultados;
    while(it!=treeName.end()&&it->first.find(ncomun)==0){
        resultados.push_back(it->second);
        it++;
    }
    return resultados;
}

int ControlParques::contarArbolesBarrio(int num_barrio){
    multimap<string,Arbol*>::iterator it=treeName.begin();
    int contador=0;
    while(it!=treeName.end()){
        if(it->second->get_num_barrio()==num_barrio){
            contador++;
        }
        it++;
    }
    return contador;
}

vector<Arbol*> ControlParques::buscarArbolesPorEspecieYDistrito(const string& nbreComun, int numDistrito){
    pair<multimap<string, Arbol*>::iterator, multimap<string, Arbol*>::iterator> pareja = treeName.equal_range(nbreComun);
    vector<Arbol*> resultados;
    while(pareja.first!=pareja.second){
        if(pareja.first->second->get_num_distrito()==numDistrito)
        resultados.push_back(pareja.first->second);
        pareja.first++;
    }
return resultados;
}
vector<Arbol*> ControlParques::arbolesCajaDistrito(string ndistrito){
    set<Arbol*> arbolesUnicos;
    for(int i = 0; i<nests.size(); i++){
        if(nests[i].estaEnArbol()){
            Arbol* arbaux = nests[i].getArbol();
            if(arbaux !=nullptr && arbaux->get_nbre_distrito()==ndistrito){
                arbolesUnicos.insert(arbaux);
            }
        }
    }
    return vector<Arbol*>(arbolesUnicos.begin(),arbolesUnicos.end());
}

unsigned int ControlParques::contarCajasEnArbol(){
    unsigned int contador = 0;
    for (int i =0; i<nests.size(); i++){
        if (nests[i].estaEnArbol()){
            contador++;
        }
    }
    return contador;
}

vector<CajaNido*> ControlParques::cajasEnDistrito(string nbre_distrito){
    vector<CajaNido*> resultados;
    for(int i=0;i<nests.size();i++){
        if(nests[i].getDistrito().find(nbre_distrito)!=string::npos){
            resultados.push_back(&nests[i]);
        }
    }
    return resultados;
}

unsigned int ControlParques::idCajaNido(const CajaNido* caja) const {
    if (caja == nullptr) {
        return 0;
    }
    
    for (unsigned int i = 0; i < nests.size(); ++i) {
        if (&nests[i] == caja) {
            return i + 2; 
        }
    }
    
    return 0;
}

CajaNido* ControlParques::buscarCajaNido(unsigned int linea) {
if (linea<2 || (linea-2)>=nests.size()){
    return nullptr;
}
return &nests[linea - 2];
}

vector<Parque*> ControlParques::parquesConCajaNido() {
    set<Parque*> parquesUnicos;
    for(int i = 0; i<nests.size();i++){
        if(nests[i].estaEnArbol()){
            Arbol* arbaux = nests[i].getArbol();
            if(arbaux->getParque()!=nullptr){
                parquesUnicos.insert(arbaux->getParque());
            }
        }
    }
    return vector<Parque*>(parquesUnicos.begin(),parquesUnicos.end());
}