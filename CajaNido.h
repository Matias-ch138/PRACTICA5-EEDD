#ifndef CAJANIDO_H
#define CAJANIDO_H

#include <iostream>
#include <string>
#include "UTM.h"
#include "Arbol.h"
using namespace std;

class CajaNido{
private:
    string distrito_; 
    string barrio_;
    string ubicacion_;
    UTM coord_;
    Arbol* itstree_;
public:
    CajaNido(string distrito, string barrio, string ubicacion, UTM& coord);
    ~CajaNido()=default;
    string getDistrito() const;
    void setDistrito(const std::string& dist);

    string getBarrio() const;
    void setBarrio(const std::string& barr);

    string getUbicacion() const;
    void setUbicacion(const std::string& ubi);
    
    UTM getCoord() const;
    void setCoord(const UTM& c);

    void setArbol(Arbol* a);
    Arbol* getArbol() const;
    bool estaEnArbol() const;
};
#endif