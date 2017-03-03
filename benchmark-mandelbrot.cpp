#include <iostream>
#include <assert.h>
#include "Mandelbrot.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"

using namespace tbb;

int main(int argc, char *argv[]) {

    // On obtient les arguments.
    assert( argc > 2 );
    int width = atoi( argv[1] );
    int height = atoi( argv[2] );

    int iter_max = 500;
    if ( argc > 3 ) {
        // L'itération maximale est optionnelle.
        iter_max = atoi( argv[3] );
    }

    Mandelbrot *mandel = new Mandelbrot(width, height, iter_max, 0.06);
    MandelbrotDataModel mandelbrot_graph_par = mandel->initDataModel(0.2,0.6);
    MandelbrotDataModel mandelbrot_graph_seq = mandel->initDataModel(-2,2);;

    printf("Creating a mandelbrot set image %dx%d with %d iterations.\n", width, height, iter_max);
    int nb_threads_max = 2 * task_scheduler_init::default_num_threads();
    printf( "# nb_threads maximum = %d\n", nb_threads_max );
    printf( "# nb.th.  seq        par        acc\n" );

    // Debut de la mesure du temps d'execution sequentiel.
    tick_count debut = tick_count::now();
    mandel->process_seq(mandelbrot_graph_seq);
    tick_count fin = tick_count::now();
    double temps_s = (fin - debut).seconds();

    for( int nb_threads = 1; nb_threads <= nb_threads_max; nb_threads *= 2 ) {
        printf( "%3d  ", nb_threads );
        task_scheduler_init init( nb_threads );

        // Debut de la mesure du temps d'execution parellele.
        debut = tick_count::now();
        mandel->process_par(mandelbrot_graph_par);
        fin = tick_count::now();
        float temps_p = (fin - debut).seconds();

        // Impression des resultats.
        printf( "%10.3f ", temps_s );
        printf( "%10.3f ", temps_p );
        printf( "%10.3f ", temps_s/temps_p );
        printf( "\n" );
    }

    mandel->drawGraph(mandelbrot_graph_par, "mandelbrot_ppm_par.ppm");
    mandel->drawGraph(mandelbrot_graph_seq, "mandelbrot_ppm_seq.ppm");

    return 0;
}