                          /* geo.h : la geographie */

#ifndef GEO_H
#define GEO_H

/* On utilise NBVILLES (genereCarte) */

/**********************************************************************/
/********* Les Villes *************************************************/
/**********************************************************************/

typedef struct { int x;
                 int y;
               }        Ville;

extern void   genereVille(Ville *ville, int coteCarte);
extern void   printVille(const Ville *ville);
extern void   dessineVille(FILE* flot, const Ville *ville);
extern void   dessineUneSeuleVille(FILE* flot, const Ville *ville);
extern double distanceVilles(const Ville *ville1, const Ville *ville2);

/**********************************************************************/
/********* Une Carte **************************************************/
/**********************************************************************/

typedef struct { Ville  villes[NBVILLES];
                 int    nbVilles;
               }                          Carte;

extern void genereCarte(Carte *carte, int coteCarte);
extern void printCarte(const Carte *carte);
extern void dessineCarte(FILE* flot,const Carte *carte);

#endif /* GEO_H */
