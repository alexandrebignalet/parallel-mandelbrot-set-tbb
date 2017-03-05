#include <cmath>
#include "./include/Point.h"
#include <stdio.h>
using namespace std;

Point::Point(double x, double y) {
    this->x_pos = x;
    this->zx = x;
    this->y_pos = y;
    this->zy = y;
    this->last_iter = 0;
}

// zn+1 = zn^2 + c
// Dealing with complex nos
void Point::next() {
    double old_zx = this->zx;
    this->zx = (this->zx * this->zx) - (this->zy * this->zy) + this->x_pos;
    this->zy = (2 * old_zx * this->zy) + this->y_pos;
    this->last_iter++;
}

bool Point::isWithinCardioid() {
    // double b = (( this->zx + 1 ) * ( this->zx + 1 )) + (this->zy * this->zy);

    double p = sqrt( ((this->zx - 0.25)*(this->zx - 0.25))+(this->zy*this->zy) );
    double c = p - (2*p*p) + 0.25;

    return this->zx < c;

}

double Point::abs() {
    return sqrt ((this->zx * this->zx) + (this->zy * this->zy));
}
//Absolute value squared
double Point::absSq() {
    return (this->zx * this->zx) + (this->zy * this->zy);
}

int Point::getLastIter() {
    return this->last_iter;
}
