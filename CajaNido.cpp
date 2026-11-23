#include "CajaNido.h"

CajaNido::CajaNido(string distrito, string barrio, string ubicacion, UTM& coord): distrito_(distrito),barrio_(barrio),ubicacion_(ubicacion),coord_(coord), itstree_(nullptr) {}
string CajaNido::getDistrito() const{
    return this->distrito_;
}
void CajaNido::setDistrito(const std::string& dist){
    this->distrito_=dist;
}

string CajaNido::getBarrio() const{
    return this->barrio_;
}
void CajaNido::setBarrio(const std::string& barr){
    this->barrio_=barr;
}

string CajaNido::getUbicacion() const{
    return this->ubicacion_;
}
void CajaNido::setUbicacion(const std::string& ubi){
    ubicacion_=ubi;
}
    
UTM CajaNido::getCoord() const{
    return this->coord_;
}
void CajaNido::setCoord(const UTM& c){
    this->coord_=c;
}

void CajaNido::setArbol(Arbol* a){
    this->itstree_=a;
}
Arbol* CajaNido::getArbol() const{
    return this->itstree_;
}
bool CajaNido::estaEnArbol() const{
    return itstree_ != nullptr;
}