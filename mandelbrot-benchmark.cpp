#include <iostream>
#include <assert.h>
#include "./src/include/Mandelbrot.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"

using namespace tbb;

int main(int argc, char *argv[])
{

    // On obtient les arguments.
    assert( argc > 2 );
    int width = atoi( argv[1] );
    int height = atoi( argv[2] );

    int iter_max = 500;
    if ( argc > 3 ) {
        // L'itération maximale est optionnelle.
        iter_max = atoi( argv[3] );
    }

    const int warming_up_nb = 5;
    const int repetition_nb = 5;

    Mandelbrot *mandel = new Mandelbrot(width, height, iter_max, 4.0);

    printf("Creating a mandelbrot set image %dx%d with %d iterations.\n", width, height, iter_max);


    int nb_threads_max = 2 * task_scheduler_init::default_num_threads();

    printf( "# WARMUP_NB = %d\n", warming_up_nb );
    printf( "# REPETITION_NB = %d\n", repetition_nb );
    printf( "# MAXIMUM THREADS_NB = %d\n", nb_threads_max );
    printf( "# nb.th.  seq        par_dyn    par_static    par   acc_dyn    acc_static    acc_par\n" );




    tick_count debut;
    tick_count fin;
    double temps_s = 0;

    double temps_par_dyn = 0;
    double temps_par_static = 0;
    double temps_par = 0;

    double temps_seq_sum = 0;
    double temps_par_dyn_sum = 0;
    double temps_par_static_sum = 0;
    double temps_par_sum = 0;

    MandelbrotDataModel mandelbrot_graph_seq;
    MandelbrotDataModel mandelbrot_graph_par_dyn;
    MandelbrotDataModel mandelbrot_graph_par_static;
    MandelbrotDataModel mandelbrot_graph_par;

    for( int nb_threads = 1; nb_threads <= nb_threads_max; nb_threads *= 2 ) {
        temps_seq_sum = 0;
        temps_par_dyn_sum = 0;
        temps_par_static_sum = 0;
        temps_par_sum = 0;

        task_scheduler_init init( nb_threads );

        for (int warming = 0; warming < warming_up_nb; warming++) {

            mandelbrot_graph_seq = mandel->initDataModel(-2,2);
            mandelbrot_graph_par_dyn = mandel->initDataModel(-2,2);
            mandelbrot_graph_par_static = mandel->initDataModel(-2,2);
            mandelbrot_graph_par= mandel->initDataModel(-2,2);

            mandel->process_seq(mandelbrot_graph_seq);
            mandel->process_par_dyn(mandelbrot_graph_par_dyn);
            mandel->process_par(mandelbrot_graph_par);
            mandel->process_par_static(mandelbrot_graph_par_static, mandelbrot_graph_par_static.begin(),
                                       mandelbrot_graph_par_static.end(), 1);
        }

        for ( int rep = 0; rep < repetition_nb; rep++) {

          mandelbrot_graph_seq = mandel->initDataModel(-2,2);
          mandelbrot_graph_par_dyn = mandel->initDataModel(-2,2);
          mandelbrot_graph_par_static = mandel->initDataModel(-2,2);
          mandelbrot_graph_par = mandel->initDataModel(-2,2);

          // Debut de la mesure du temps d'execution sequentiel.
          debut = tick_count::now();
          mandel->process_seq(mandelbrot_graph_seq);
          fin = tick_count::now();
          temps_s = (fin - debut).seconds();

          // Debut de la mesure du temps d'execution parellele avec repartition dynamique vol de taches.
          debut = tick_count::now();
          mandel->process_par_dyn(mandelbrot_graph_par_dyn);
          fin = tick_count::now();
          temps_par_dyn = (fin - debut).seconds();

          debut = tick_count::now();
          mandel->process_par(mandelbrot_graph_par);
          fin = tick_count::now();
          temps_par = (fin - debut).seconds();

          // Debut de la mesure du temps d'execution parellele avec repartition statique.
          debut = tick_count::now();
          mandel->process_par_static(mandelbrot_graph_par_static, mandelbrot_graph_par_static.begin(),
                                     mandelbrot_graph_par_static.end(), 1);
          fin = tick_count::now();
          temps_par_static = (fin - debut).seconds();

          temps_par_dyn_sum += temps_par_dyn;
          temps_seq_sum += temps_s;
          temps_par_static_sum += temps_par_static;
          temps_par_sum += temps_par;
        }

        double seq_avg = temps_seq_sum / repetition_nb;
        double par_dyn_avg = temps_par_dyn_sum / repetition_nb;
        double par_static_avg = temps_par_static_sum / repetition_nb;
        double par_avg = temps_par_sum / repetition_nb;

        // Impression des resultats.
        printf( "%3d  ", nb_threads );
        printf( "%10.3f ", seq_avg );

        printf( "%10.3f ", par_dyn_avg );
        printf( "%10.3f ", par_static_avg );
        printf( "%10.3f ", par_avg );

        printf( "%10.1f ", seq_avg/par_dyn_avg );
        printf( "%10.1f ", seq_avg/par_static_avg );
        printf( "%10.1f ", seq_avg/par_avg );
        printf( "\n" );
    }

    mandel->drawGraph(mandelbrot_graph_par_dyn, "./images/mandelbrot_ppm_par_dyn.ppm");
    mandel->drawGraph(mandelbrot_graph_par_static, "./images/mandelbrot_ppm_par_static.ppm");
    mandel->drawGraph(mandelbrot_graph_seq, "./images/mandelbrot_ppm_seq.ppm");
    mandel->drawGraph(mandelbrot_graph_par, "./images/mandelbrot_ppm_par.ppm");

    return 0;
}
