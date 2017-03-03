#include <cmath>
#include "Point.h"

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