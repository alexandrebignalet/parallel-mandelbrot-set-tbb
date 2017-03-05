#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <cmath>
#include "./lib/MiniCUnit.h"
#include "../src/include/Mandelbrot.h"

static SuiteDeTests suiteMandelbrot();

int main( int argc, char* argv[] )
{
  executerSuiteDeTests( suiteMandelbrot );

  printf( "\n%s", sommaireDeTests() );

  return 0;
}

bool point_egaux(Point &p, Point &p2) {
  return p.x_pos == p2.x_pos &&
         p.y_pos == p2.y_pos &&
         p.zx == p2.zx &&
         p.zy == p2.zy &&
         p.last_iter == p2.last_iter;
}

bool mandelbrot_egaux(MandelbrotDataModel m1, MandelbrotDataModel m2) {
  if ( !(m1.size() == m2.size() && m1[0].size() != m2[0].size()) ) {
    return false;
  }


  for (unsigned int i = 0; i < m1.size(); i++) {
    for (unsigned int y = 0; y < m1[0].size(); y++) {
      if (!point_egaux(m1[i][y], m2[i][y]) ) {
        return false;
      }
    }
  }

  return true;
}

// Cas de tests Mandelbrot(int width, int height, int iter_max, double zoom)
// Ce constructeur permet d'instancier l'objet permettant de créer les fractales
// il définit la taille de l'image (longueur/ largeur)
// le nombre d'itérations maximales afin de savoir si un point appartient à
// l'ensemble ou non et le zoom sur l'image
CAS_DE_TEST( test_constructor )
Mandelbrot *mandel = new Mandelbrot(640, 360, 200, 4.0);

assertVrai(mandel->image_height == 360);
assertVrai(mandel->image_width == 640);
assertVrai(mandel->iter_max == 200);
assertVrai(mandel->x_scale == 4.0 / 640);
assertVrai(mandel->y_scale == 4.0 / 360);

FIN_CAS_DE_TEST

// Cas de tests initDataModel(x_limit, y_limit)
// crée un MandelbrotDataModel représentant le plan dimensionné à la création
// de l'objet.
// Ce plan contient tous les points de coordonnées entières comprises
// entre x_limit et y_limit
CAS_DE_TEST( test_init_data_model )
Mandelbrot *mandel = new Mandelbrot(640, 360, 200, 4.0);
MandelbrotDataModel plan = mandel->initDataModel(-2,2);

for ( int i = 0; i < plan.size(); i++) {
  for (int y = 0; y < plan[0].size(); y++) {
    assertVrai( -2 + (y * mandel->x_scale) == plan[i][y].x_pos );
    assertVrai( 2 - (i * mandel->y_scale) == plan[i][y].y_pos );
  }
}

FIN_CAS_DE_TEST

// Cas de tests la version sequentielle génère bien la fractale voulue.
// Pour tester les versions parallele on comparera les MandelbrotDataModel resultants
CAS_DE_TEST( test_generation )
Mandelbrot *mandel = new Mandelbrot(640, 360, 200, 4.0);
MandelbrotDataModel graph_seq = mandel->initDataModel(-2,2);
MandelbrotDataModel graph_par_dyn = mandel->initDataModel(-2,2);
MandelbrotDataModel graph_par_static = mandel->initDataModel(-2,2);
MandelbrotDataModel graph_par = mandel->initDataModel(-2,2);

mandel->process_seq(graph_seq);
mandel->process_par_dyn(graph_par_dyn);
mandel->process_par_static(graph_par_static, graph_par_static.begin(), graph_par_static.end(), 1);
mandel->process_par(graph_par);

mandelbrot_egaux(graph_seq, graph_par_dyn);
mandelbrot_egaux(graph_seq, graph_par_static);
mandelbrot_egaux(graph_seq, graph_par);

FIN_CAS_DE_TEST

SUITE_DE_TESTS( suiteMandelbrot )
    ajouterCasDeTest( test_constructor );
    ajouterCasDeTest( test_init_data_model );
    ajouterCasDeTest( test_generation );

FIN_SUITE_DE_TESTS
