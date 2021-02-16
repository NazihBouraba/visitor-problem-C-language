                            /* geo.c : la geographie */
#include <stdio.h>
#include <math.h>           /* Pour sqrt */
#include <stdlib.h>         /* Pour malloc, free */

#include "params.h"
#include "geo.h"
#include "random.h"
#include "gnuplot.h"

/* On utilise NBVILLES (genereCarte) */


/**********************************************************************/
/********* Les Villes *************************************************/
/**********************************************************************/
/*
typedef struct { int x;
                 int y;
               }        Ville;
*/

void genereVille(Ville *ville, int coteCarte)
{
 ville->x=myRandomMinMax(0,coteCarte-1);
 ville->y=myRandomMinMax(0,coteCarte-1);
}

void printVille(const Ville *ville)
{
 printf("{%d,%d}",ville->x,ville->y);
}

void dessineVille(FILE* flot,const Ville *ville)
{
 vectorGnuplot(flot,ville->x,ville->y,ville->x,ville->y);
}

void dessineUneSeuleVille(FILE* flot, const Ville *ville)
{
 beginPointsToGnuplot(flot,"linespoint");
 vectorGnuplot(flot,ville->x,ville->y,ville->x,ville->y);
 endPointsToGnuplot(flot);
}

double distanceVilles(const Ville *ville1, const Ville *ville2)
{
 int difX = ville1->x - ville2->x;
 int difY = ville1->y - ville2->y;

 return( sqrt(difX*difX + difY*difY) );
}

/**********************************************************************/
/********* Une Carte **************************************************/
/**********************************************************************/
/*
typedef struct { Ville  villes[NBVILLES];
                 int    nbVilles;
               }                          Carte;
*/

void genereCarte(Carte *carte, int coteCarte)
{
 int    nbVillesGenerees=0;

 carte->nbVilles=NBVILLES;

 nbVillesGenerees=0;
 while (nbVillesGenerees!=carte->nbVilles)
 {
   Ville ville;
   int   i=0,trouve=0;

   genereVille(&ville,coteCarte);

   /* Il faut rechercher si la ville est deja presente dans la carte */

   for(i=0;i<nbVillesGenerees;i++)
   {
    if ((carte->villes[i].x==ville.x) &&
        (carte->villes[i].y==ville.y)) trouve=1;
   }
   if (!trouve) carte->villes[nbVillesGenerees++]=ville;
 }

}

void printCarte(const Carte *carte)
{
 int i=0;
 for(i=0;i<carte->nbVilles;i++)
 {
  printVille((const Ville*)&(carte->villes[i]));
  if(i!=carte->nbVilles-1) printf(",");
  if (((i+1)%5==0) || (i==carte->nbVilles-1)) printf("\n");
 }
}

void dessineCarte(FILE* flot, const Carte *carte)
{
 int i=0;
 beginPointsToGnuplot(flot,"linespoint");
 for(i=0;i<carte->nbVilles;i++)
 {
  dessineVille(flot,(const Ville*)&(carte->villes[i]));
 }
 endPointsToGnuplot(flot);
}
