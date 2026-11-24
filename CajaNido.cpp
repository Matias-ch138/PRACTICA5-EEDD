#include "CajaNido.h"
/**
 * @brief constructor de un objeto tipo CajaNido
 * 
 * @param distrito distrito en el que esta
 * @param barrio barrio en el que esta
 * @param ubicacion ubicación en el que esta
 * @param coord coordenadas en el que esta
 */
CajaNido::CajaNido(string distrito, string barrio, string ubicacion, UTM& coord): distrito_(distrito),barrio_(barrio),ubicacion_(ubicacion),coord_(coord), itstree_(nullptr) {}
/**
 * @brief obtener el distrito en el que se encuentra la CajaNido
 * 
 * @return string distrito en el que se encuentra la CajaNido
 */
string CajaNido::getDistrito() const{
    return this->distrito_;
}
/**
 * @brief establecer el distrito en el que se encuentra la CajaNido
 * 
 * @param dist distrito que queremos establecer
 */
void CajaNido::setDistrito(const std::string& dist){
    this->distrito_=dist;
}
/**
 * @brief obtener el barrio en el que se encuentra la CajaNido
 * 
 * @return string barrio en el que se encuentra la CajaNido
 */
string CajaNido::getBarrio() const{
    return this->barrio_;
}
/**
 * @brief establecer el barrio en el que se encuentra la CajaNido
 * 
 * @param barr barrio que queremos establecer
 */
void CajaNido::setBarrio(const std::string& barr){
    this->barrio_=barr;
}
/**
 * @brief Obtener la ubicación de la CajaNido
 * 
 * @return string ubicación de la CajaNido
 */
string CajaNido::getUbicacion() const{
    return this->ubicacion_;
}
/**
 * @brief establecer la ubicación de la CajaNido
 * 
 * @param ubi ubicación que queremos establecer
 */
void CajaNido::setUbicacion(const std::string& ubi){
    ubicacion_=ubi;
}
/**
 * @brief obtener las coordenadas de la CajaNido
 * 
 * @return UTM coordenadas de la CajaNido
 */
UTM CajaNido::getCoord() const{
    return this->coord_;
}
/**
 * @brief Establecer las coordenadas dela CajaNido
 * 
 * @param c coordendas de la CajaNido
 */
void CajaNido::setCoord(const UTM& c){
    this->coord_=c;
}
/**
 * @brief establecer un arbol a la CajaNido
 * 
 * @param a arbol que se quiere establecer
 */
void CajaNido::setArbol(Arbol* a){
    this->itstree_=a;
}
/**
 * @brief Obtener el arbol asociado a la CajaNido
 * 
 * @return Arbol* arbol asociado a la CajaNido
 */
Arbol* CajaNido::getArbol() const{
    return this->itstree_;
}
/**
 * @brief Saber si tiene asociación una CajaNido con un Arbol
 * 
 * @return true si tiene asociación
 * @return false si no tiene asociación
 */
bool CajaNido::estaEnArbol() const{
    return itstree_ != nullptr;
}