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

    const int warming_iter = 5;

    Mandelbrot *mandel = new Mandelbrot(width, height, iter_max, 4.0);

    printf("Creating a mandelbrot set image %dx%d with %d iterations.\n", width, height, iter_max);
    int nb_threads_max = 2 * task_scheduler_init::default_num_threads();
    printf( "# nb_threads maximum = %d\n", nb_threads_max );
    printf( "# nb.th.  seq        par_dyn    par_static   acc_dyn    acc_static\n" );

    MandelbrotDataModel mandelbrot_graph_seq = mandel->initDataModel(-2,2);

    // Debut de la mesure du temps d'execution sequentiel.
    tick_count debut = tick_count::now();
    mandel->process_seq(mandelbrot_graph_seq);
    tick_count fin = tick_count::now();
    double temps_s = (fin - debut).seconds();
    double temps_par_dyn = 0;
    double temps_par_static = 0;
    double temps_par_dyn_sum = 0;
    double temps_par_static_sum = 0;

    MandelbrotDataModel mandelbrot_graph_par_dyn;
    MandelbrotDataModel mandelbrot_graph_par_static;

    for( int nb_threads = 1; nb_threads <= nb_threads_max; nb_threads *= 2 ) {
        for (int warming = 0; warming < warming_iter; warming++) {

            mandelbrot_graph_par_dyn = mandel->initDataModel(-2,2);
            mandelbrot_graph_par_static = mandel->initDataModel(-2,2);

            printf( "%3d  ", nb_threads );
            task_scheduler_init init( nb_threads );

            // Debut de la mesure du temps d'execution parellele avec repartition dynamique vol de taches.
            debut = tick_count::now();
            mandel->process_par_dyn(mandelbrot_graph_par_dyn);
            fin = tick_count::now();
            temps_par_dyn = (fin - debut).seconds();

            // Debut de la mesure du temps d'execution parellele avec repartition statique.
            debut = tick_count::now();
            mandel->process_par_static(mandelbrot_graph_par_static, mandelbrot_graph_par_static.begin(),
                                       mandelbrot_graph_par_static.end(), 1);
            fin = tick_count::now();
            temps_par_static = (fin - debut).seconds();

            // Impression des resultats.
            printf( "%10.3f ", temps_s );
            printf( "%10.3f ", temps_par_dyn );
            printf( "%10.3f ", temps_par_static );
            printf( "%10.1f ", temps_s/temps_par_dyn );
            printf( "%10.1f ", temps_s/temps_par_static );
            printf( "\n" );
        }
        temps_par_dyn_sum += temps_par_dyn;
        temps_par_static_sum += temps_par_static;

        printf("# avg par_dyn: %.3f \n", temps_par_dyn_sum / warming_iter);
        printf("# avg par_static: %.3f \n", temps_par_static_sum / warming_iter);
    }

    mandel->drawGraph(mandelbrot_graph_par_dyn, "mandelbrot_ppm_par_dyn.ppm");
    mandel->drawGraph(mandelbrot_graph_par_static, "mandelbrot_ppm_par_static.ppm");
    mandel->drawGraph(mandelbrot_graph_seq, "mandelbrot_ppm_seq.ppm");

    return 0;
}