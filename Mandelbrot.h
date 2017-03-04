//
// Created by alexandre on 3/1/17.
//
#include <vector>
#include <string>

using namespace std;

#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H

#include "Point.h"

typedef vector<vector<Point>> MandelbrotDataModel;

struct Mandelbrot {
    int iter_max;

    int image_width;
    int image_height;

    double x_scale;
    double y_scale;


    Mandelbrot(int width, int height, int iter_max, double zoom);

    void process_line(vector<Point>& mandel_line);
    void process_seq(MandelbrotDataModel& mandel);
    void process_par(MandelbrotDataModel& mandel);
    void process_par_dyn(MandelbrotDataModel& mandel);
    void process_par_static(MandelbrotDataModel& mandel,
                            vector<vector<Point>>::iterator left,
                            vector<vector<Point>>::iterator right,
                            int seuil);
    MandelbrotDataModel initDataModel(double x_limit, double y_limit);
    void drawGraph(MandelbrotDataModel mandel, string name);
};


#endif //MANDELBROT_MANDELBROT_H
