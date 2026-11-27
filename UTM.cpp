#include "UTM.h"

/**
 * @brief Constructor de un objeto UTM
 * 
 * @param x coordenada x
 * @param y coordenada y
 */
UTM::UTM(double x, double y): X(x),Y(y){}
/**
 * @brief obtener la cordenada x
 * 
 * @return double coordenada x
 */
double UTM::getX() const{
return X;
}
/**
 * @brief obtener la cordenada y
 * 
 * @return double coordenada y
 */
double UTM::getY() const{
    return Y;
}
/**
 * @brief establecer la coordenada x
 * 
 * @param x coordenada x
 */
void UTM::setX(double x){
    this->X=x;
}
/**
 * @brief establecer la coordenada y
 * 
 * @param y coordenada y
 */
void UTM::setY(double y){
    this->Y=y;
}
/**
 * @brief distancia euclidea entre dos objetos del tipo UTM
 * 
 * @param otro objeto UTM que se va a comparar
 * @return double distancia entre los dos objetos
 */
double UTM::distancia2(const UTM &otro) const{
    double dx = this->X - otro.X;
    double dy = this->Y - otro.Y;
    return (dx * dx) + (dy * dy);
}