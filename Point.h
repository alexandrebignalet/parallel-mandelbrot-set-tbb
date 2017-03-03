//
// Created by alexandre on 3/1/17.
//

#ifndef MANDELBROT_POINT_H_H
#define MANDELBROT_POINT_H_H
struct Point
{
    //coordonées reelles
    double x_pos;
    double y_pos;
    // coordonées plan complexe
    //partie reelle
    double zx;
    //partie imaginaire
    double zy;

    int last_iter;

    Point(double x, double y);

    // zn+1 = zn^2 + c
    // Dealing with complex nos
    void next();

    double abs();
    //Absolute value squared
    double absSq();

    int getLastIter();
};
#endif //MANDELBROT_POINT_H_H
