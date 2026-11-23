#include "UTM.h"


UTM::UTM(double x, double y): X(x),Y(x){}

double UTM::getX() const{
return X;
}
double UTM::getY() const{
    return Y;
}

void UTM::setX(double x){
    this->X=x;
}
void UTM::setY(double y){
    this->Y=y;
}

double UTM::distancia2(const UTM &otro){
    double dx = this->X - otro.X;
    double dy = this->Y - otro.Y;
    return (dx * dx) + (dy * dy);
}