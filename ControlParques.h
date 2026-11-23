#ifndef CONTROLPARQUES_H
#define CONTROLPARQUES_H

#include <iostream>
#include <string>
using namespace std;

#include "Arbol.h"
#include "Especie.h"
#include "Parque.h"
#include "ThashArbol.h"
#include "CajaNido.h"
#include <list>

class ControlParques {
private:
  map<string, Especie> varieties;
  map<int, Parque> parks;
  ThashArbol trees; //tabla hash de arboles
  multimap<string, Arbol*> treeName; //clave nombrecomun
  vector<CajaNido> nests;

public:
  /**
     * @brief Constructor por defecto de la clase ControlParques
     * 
     */
  ControlParques();
  /**
     * @brief Constructor parametrizado de la clase ControlParques
     * 
     * @param rutaEspecies ruta del archivo que contiene las especies
     * @param rutaParques ruta del archivo que contienen los parques
     * @param rutaArboles ruta del archivo que contiene los arboles
     * @param rutaCajasNido ruta del archivo que contiene las cajas nido
     */
  ControlParques(const string& rutaEspecies, const string& rutaParques, const string& rutaArboles, const string& rutaCajasNido);
  /**
     * @brief Cuenta el número de especies que hay en todos los parques dado un nombre comun
     * 
     * @param ncomun el nombre comun de las especies que queremos contar
     * @return int el número de especies con @param ncomun que hay en todos los parques
     */
  int contarEspecieParque(string ncomun);
  /**
     * @brief Genera un vector de todas las especies con un subnombre especifico
     * 
     * @param cadena subnombre que buscamos
     * @return vector<Especie> Vector de especies con ese subnombre
     */
  vector<Especie> listadoEspecieSubNbre(string cadena);
  /**
     * @brief Busqueda de un parque dado su nombre
     * 
     * @param nbre_parque nombre del parque que buscamos
     * @return Parque* puntero al parque con nombre @param nbre_parque
     */
  Parque* buscarParque(const string& nbre_parque);

  /**
     * @brief Busqueda de un arbol dado su @param asNum
     * 
     * @param asNum asNum del arbol que buscamos
     * @return Arbol* puntero al arbol con @param asNum
     */
  Arbol* buscarArbol(string asNum);
  /**
     * @brief Dado un nombre de un parque genera un vector con todos los arboles que tiene
     * 
     * @param nbre_parque nombre del parque
     * @return vector<Arbol*> todos los arboles que se encuentran en el parque cuyo nombre es @param nbre_parque
     */
  vector<Arbol*> listaArbParque(string nbre_parque);
  /**
     * @brief Nos da el arbol mas alto de todos los parques
     * 
     * @return Arbol* puntero al arbol mas alto
     */
  Arbol* arbolMasAlto();
  /**
     * @brief Segun el nombre del parque cuenta las especies distintas que tiene
     * 
     * @param nbre_parque nombre del parque
     * @return int número de especies distintas que tiene el parque
     */
  int contarEspecies(string nbre_parque);
  /**
     * @brief Cuenta todos los tipos de especies diferentes alojados en todos los parques
     * 
     * @return int número de tipos de especies diferentes alojados en todos los parques
     */
  int contarTipos();
  /**
     * @brief Devuelve el número de árboles almacenados en el AVL.
     * @return unsigned int cantidad total de árboles cargados.
     */
  unsigned int numArboles() const;
  /**
     * @brief obtener las especies de todos los parques
     * 
     * @return vector<Especie>& vector dinamico con todas las especies de todos los parques
     */
  map<string,Especie>& get_varieties();
  /**
     * @brief obtener las especies de todos los parques
     * 
     * @return const vector<Especie>& vector dinamico con todas las especies de todos los parques
     */
  const map<string,Especie>& get_varieties() const;
  /**
     * @brief obtener todos los parques
     * 
     * @return list<Parque>& Lista Doblemente Enlazada con todos los parques
     */
  map<int, Parque>& get_parks();
  /**
     * @brief obtener todos los parques
     * 
     * @return const list<Parque>& Lista Doblemente Enlazada con todos los parques
     */
  const map<int, Parque>& get_parks() const;
  /**
     * @brief obtner todos los parques
     * 
     * @return list<Parque>& Lista Doblemente Enlazada con todos los parques
     */
  map<int, Parque>& get_nbre_parque();
  /**
     * @brief obtner todos los parques
     * 
     * @return const list<Parque>& Lista Doblemente Enlazada con todos los parques
     */
  const map<int, Parque>& get_nbre_parque() const;
  /**
   * @brief obtener las cajas nido
   * 
   * @return vector<CajaNido>& vector con cajas nido
   */
  vector<CajaNido>& get_nests();
  /**
   * @brief obtener las cajas nido
   * 
   * @return const vector<CajaNido>& vector con cajas nido
   */
  const vector<CajaNido>& get_nests() const;
  /**
   * @brief obtener el tamaño de la tabla hash de arboles
   * 
   * @return unsigned int tamaño tabla hash de arboles
   */
  unsigned int tamTabla() const;
  /**
   * @brief redispersar la tabla hash con un tamaño nuevo
   * 
   * @param tam tamaño con el que queremos redispersar la tabla hash
   */
  void redispersar(unsigned int tam);
  /**
   * @brief muestra el estado de la tabla hash
   * 
   */
  void mostrarEstadoTabla() const;
  /**
     * @brief devuelve un conjunto de parques que tienen al menos
      un ejemplar del árbol dado por parámetro mediante su nombre común
     * 
     * @param ncomun nombre comun del arbol 
     * @return vector<Parque> conjunto de parques que tiene al menos un ejemplar del árbol dado
     */
  vector<Parque*> listarParquesArbol(const string& ncomun);
  /**
     * @brief localiza en tiempo óptimo todos los ejemplares de
      árboles con ese nombre común exacto
     * 
     * @param ncomun nombre comun del arbol 
     * @return vector<Arbol*> todos los arboles con ese nombre comun exacto
     */
  vector<Arbol*> buscarArbolNcomun(const string& ncomun);
  /**
     * @brief localiza en tiempo óptimo todos los
      ejemplares de árboles dando una palabra del comienzo
     * 
     * @param ncomun subcadena del nombre comun del arbol
     * @return vector<Arbol*> todos los arboles con esa subcadena de nombre comun
     */
  vector<Arbol*> buscarArbolNcomunComienzo(const string& ncomun);
  /**
     * @brief devuelve el número de árboles que tiene un barrio
      de Madrid dado su código
     * 
     * @param num_barrio numero del barrio
     * @return int número de arboles en un barrio
     */
  int contarArbolesBarrio(int num_barrio);
  /**
     * @brief devuelve un listado de los árboles
      de una especie dada por su nombre común y que pertenecen a un mismo distrito
     * 
     * @param nbreComun nombre común del arbol
     * @param numDistrito número del distrito
     * @return vector<Arbol*> vector con arboles del distrito @param numDistrito y nombre @param nbreComun
     */
  vector<Arbol*> buscarArbolesPorEspecieYDistrito(const string& nbreComun, int numDistrito);


  /**
    * @brief devuelve todos los árboles del distrito indicado que tengan al menos una caja nido asociada.
    * 
    * @param ndistrito número del distrito
    * @return vector<Arbol*> vector con todos los árboles del distrito que tienen al menos una caja nido asociada.
    */
  vector<Arbol*> arbolesCajaDistrito(string ndistrito);
  /**
    * @brief devuelve el número total de cajas nido que tiene un árbol asociado
    * 
    * @return unsigned int número total de cajas nido que tiene un árbol asociado
    */
  unsigned int contarCajasEnArbol();
  /**
    * @brief devuelve todas las cajas nido ubicadas en el distrito indicado, independientemente de si tienen un árbol asociado.
    * 
    * @param nbre_distrito nombre del distrito
    * @return vector<CajaNido*> todas las cajas nido ubicadas en @param nbre_distrito independientemente de si tienen un árbol asociado.
    */
  vector<CajaNido*> cajasEnDistrito(string nbre_distrito);
  unsigned long djb2(const char* str);

    /**
     * @brief devuelve la id de una caja nido
     * 
     * @param caja la caja nido
     * @return unsigned int id de la caja nido
     */
    unsigned int idCajaNido(const CajaNido* caja) const;
    /**
     * @brief busca una caja nido por la linea del fichero .csv
     * 
     * @param linea linea del fichero .csv
     * @return CajaNido* la caja nido en esa @param linea del fichero .csv
     */
    CajaNido* buscarCajaNido(unsigned int linea);
    /**
     * @brief devuelve un vector de parques que contengan al menos una caja nido
     * 
     * @return vector<Parque*> vector de parques que contienen al menos una caja nido
     */
    vector<Parque*> parquesConCajaNido();
};


#endif