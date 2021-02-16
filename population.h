                          /* population.h : la population d'anticorps */
#ifndef POPULATION_H
#define POPULATION_H

#include "params.h"
#include "anticorps.h"

/**********************************************************************/
/********* Une Population *********************************************/
/**********************************************************************/

typedef struct { Ac *individus;     /* Tableau alloue dynamiquement d'Ac */
                 Ac *clones;        /* Pointeur vers la zone des clones  */
                 int nbIndividus;
                 int nbClones;
               }                             Population;

extern void generePopulation(Population *population,
                             int nbIndividus,int nbClones);
extern void supprimePopulation(Population *population);

/* Affichage des meilleurs individus de la Population  */
/* Pour afficher toute la population =>  nbIndividus=0 */
extern void printIndividusPopulation(Population *population,int nbIndividus);
/* Dessin des meilleurs individus de la Population     */
/* Pour dessiner toute la population =>  nbIndividus=0 */
extern void dessineIndividusPopulation(FILE* flot,
                                       Population *population,int nbIndividus);

/* Tri d'une Population avec un COUT DECROISSANT =>           */
/* les moins bon a gauche, les meilleurs sont a droite.       */
extern void triPopulation(Population *population);

/*** Il faut un tri avant (meilleur individu le plus a droite).      ***/
extern Ac  meilleurIndividu(Population *population);

/*** Il faut un tri avant (les meilleurs doivent etre a droite).     ***/
extern void clonageMeilleurs(Population *population);

/*** Il faut un clonage avant ....                                   ***/
extern void mutationClones(Population *population);

/*** Les meilleurs et les clones sont comparares                     ***/
/*** Chaque couple (meilleur,clone) est evalue...                    ***/
/*** Et on garde le meilleur des deux                                ***/
extern void selectionMeilleursEtClonesMutes(Population *population);
extern void selectionMeilleursEtClonesMutesMethod2(Population *population);

/*** Les moins bons doivent etre a gauche (apres un tri par exemple) ***/
extern void remplacementMoinsBons(Population *population, int nbNouveaux);

/*** Les moins bons doivent etre a gauche (apres un tri par exemple) ***/
extern void mutationMoinsBons(Population *population);

#endif /* POPULATION_H */
