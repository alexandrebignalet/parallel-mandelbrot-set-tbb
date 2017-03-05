//
// Created by alexandre on 3/1/17.
//

#include "./include/Mandelbrot.h"
#include <algorithm>
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_invoke.h"

using namespace tbb;

const double DIVERGENCE_LIMIT = 4.0;


tuple<int, int, int> get_rgb_smooth(int n, int iter_max) {
    // map n on the 0..1 interval
    double t = (double)n/(double)iter_max;

    // Use smooth polynomials for r, g, b
    int r = (int)(9*(1-t)*t*t*t*255);
    int g = (int)(15*(1-t)*(1-t)*t*t*255);
    int b =  (int)(8.5*(1-t)*(1-t)*(1-t)*t*255);
    return tuple<int, int, int>(r, g, b);
}

Mandelbrot::Mandelbrot(int width, int height, int iter_max, double zoom) {
    this->image_width = width;
    this->image_height = height;
    this->iter_max = iter_max;

    this->x_scale = zoom / image_width;
    this->y_scale = zoom / image_height;
}

void Mandelbrot::process_seq(MandelbrotDataModel& mandel) {

    for (int y = 0; y < this->image_height; y++) {
        this->process_line(mandel[y]);
    }
}

void Mandelbrot::process_par(MandelbrotDataModel& mandel) {

    parallel_for ( 0, this->image_height,
                 [&](int i){ this->process_line(mandel[i]); }
    );
}

void Mandelbrot::process_par_dyn(MandelbrotDataModel& mandel) {

    parallel_for( blocked_range<int>(0, this->image_height),
                  [&]( blocked_range<int> r ) {
                      for (int i = r.begin(); i < r.end(); i++ ) {
                          this->process_line(mandel[i]);
                      }
                  }
    );
}

void Mandelbrot::process_line(vector<Point>& mandel_line) {

    for_each(mandel_line.begin(), mandel_line.end(),
             [&](Point &p) {
                 if (!p.isWithinCardioid()) {
                     while ( p.absSq() < DIVERGENCE_LIMIT && p.getLastIter() < this->iter_max) {
                         p.next();
                     }
                 }
             }
    );
}

void Mandelbrot::process_par_static(
        MandelbrotDataModel& mandel,
        vector<vector<Point>>::iterator left,
        vector<vector<Point>>::iterator right,
        int seuil)
{

    if (right - left <= seuil) {

        for ( auto it = left ; it < right; ++it ) {
            auto i = std::distance(mandel.begin(), it);
            this->process_line(mandel[i]);
        }
//        for_each(left, right,
//                 [&](int line) { this->process_line(mandel[line]); }
//        );

    } else {
        auto midPosition = (right - left) / 2;
        vector<vector<Point>>::iterator middle = left + midPosition;

        parallel_invoke ( [&] { this->process_par_static(mandel, left, middle, seuil);},
                          [&] { this->process_par_static(mandel, middle, right, seuil);} );
    }
}

void Mandelbrot::drawGraph(MandelbrotDataModel mandel, string name)
{
    const char *path = name.c_str();

    FILE *fp = fopen(path, "wb");

    if (fp == NULL) {
        puts("Impossible d'ouvrir le ficher de sortie euh");
        return;
    }

    fprintf(fp, "P6\n%d %d\n255\n", this->image_width, this->image_height);

    unsigned char line_buffer[3*this->image_width];

    for (int y = 0 ; y < this->image_height; y++ ) {
        for ( int x = 0 ; x < this->image_width; x++){
            Point p = mandel[y][x];

            int iter = p.getLastIter();

            line_buffer[3*x] = (unsigned char)std::get<0>(get_rgb_smooth(iter, this->iter_max)); // Red
            line_buffer[3*x+1] = (unsigned char)std::get<1>(get_rgb_smooth(iter, this->iter_max)); // Green
            line_buffer[3*x+2] = (unsigned char)std::get<2>(get_rgb_smooth(iter, this->iter_max)); // Blue
        }
        fwrite(line_buffer, 1, sizeof(line_buffer), fp);
    }

    fclose(fp);
}



MandelbrotDataModel Mandelbrot::initDataModel(double x_limit, double y_limit) {
    MandelbrotDataModel mandel(this->image_height, vector<Point> (this->image_width, Point(0,0)));

    for (int i = 0; i < this->image_height; i++) {
        for (int j = 0; j < this->image_width; j++) {
            double y =  y_limit - (i * this->y_scale);
            double x = x_limit + (j * this->x_scale);
            mandel[i][j].y_pos = y;
            mandel[i][j].zy = y;
            mandel[i][j].x_pos = x;
            mandel[i][j].zx = x;
        }
    }

    return mandel;
}
