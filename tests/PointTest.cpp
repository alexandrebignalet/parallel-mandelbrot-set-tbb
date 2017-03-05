#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <cmath>
#include "./lib/MiniCUnit.h"
#include "../src/include/Point.h"

static SuiteDeTests suitePoints();

int main( int argc, char* argv[] )
{
  executerSuiteDeTests( suitePoints );

  printf( "\n%s", sommaireDeTests() );

  return 0;
}

// Cas de tests next() calcule le carré de l'affixe du point et met à jour ses
// coordonnées complexes avec le resultat
CAS_DE_TEST( test_next )
Point *p1 = new Point(1,1);
p1->next();
assertVrai(p1->zx == (1*1) - (1*1) + 1);
assertVrai(p1->zy == (2 * 1 * 1) + 1);

Point *p2 = new Point(-1,1);
p2->next();
assertVrai(p2->zx == (-1*-1) - (1*1) - 1);
assertVrai(p2->zy == (2 * -1 * 1) + 1);

Point *p3 = new Point(-2,1);
p3->next();
assertVrai(p3->zx == (-2*-2) - (1*1) - 2);
assertVrai(p3->zy == (2 * -2 * 1) + 1);

FIN_CAS_DE_TEST

// Cas de tests abs() calcule le module de ce point sur le plan complexe
CAS_DE_TEST( test_abs )
Point *p1 = new Point(1,1);
Point *p2 = new Point(-1,1);
Point *p3 = new Point(-2,1);

assertVrai(p1->abs() == sqrt(2));
assertVrai(p2->abs() == sqrt(2));
assertVrai(p3->abs() == sqrt(5));
FIN_CAS_DE_TEST

// abssq est le carré de abs
CAS_DE_TEST( test_absSq )
Point *p1 = new Point(1,1);
Point *p2 = new Point(-1,1);
Point *p3 = new Point(-2,1);

assertVrai(p1->absSq() == 2);
assertVrai(p2->absSq() == 2);
assertVrai(p3->absSq() == 5);
FIN_CAS_DE_TEST

// isWithinCardioid() est vrai si le point appartient à la cardioide
CAS_DE_TEST( test_is_within_cardioid )
Point *p1 = new Point(-0.7,0);
Point *p2 = new Point(0,0);
Point *p3 = new Point(-0.25,0.25);

assertVrai(p1->isWithinCardioid());
assertVrai(p2->isWithinCardioid());
assertVrai(p3->isWithinCardioid());

Point *p = new Point(2,2);
Point *p4 = new Point(-1,1);

assertFaux(p->isWithinCardioid());
assertFaux(p4->isWithinCardioid());
FIN_CAS_DE_TEST

SUITE_DE_TESTS( suitePoints )
    ajouterCasDeTest( test_next );
    ajouterCasDeTest( test_abs );
    ajouterCasDeTest( test_absSq );
    ajouterCasDeTest( test_is_within_cardioid );
FIN_SUITE_DE_TESTS
