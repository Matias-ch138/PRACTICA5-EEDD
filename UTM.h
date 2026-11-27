#include <iostream>
using namespace std;
#ifndef UTM_H
#define UTM_H
class UTM{
private:
    double X;
    double Y;
public:
    UTM(double x = 0, double y = 0);
    ~UTM()=default;
    double getX() const;
    double getY() const;

    void setX(double x);
    void setY(double y);

    double distancia2(const UTM &otro) const;
};

#endif