#include <stdio.h>
#include <float.h>      /* Pour DBL_MAX                     */
#include <unistd.h>     /* Pour sleep                       */
#include <stdlib.h>     /* Pour exit                        */

#include "params.h"
#include "geo.h"
#include "anticorps.h" 
#include "population.h"
#include "random.h"
#include "gnuplot.h"
 
/*********************************************************************/
/*****************************  DEBUT ********************************/
/*********************************************************************/
#if NBVILLES==8

 Carte carte={{{5,2},{7,3},{8,5},{7,7},{5,8},{3,7},{2,5},{3,3}}, 8};

 Ac    BestOf={{0},17.888544, NBVILLES};

#elif NBVILLES==16

 Carte carte={{{5,2},{6,2},{7,3},{8,4},
               {8,5},{8,6},{7,7},{6,8},
               {5,8},{4,8},{3,7},{2,6},
               {2,5},{2,4},{3,3},{4,2}}, 16};

 Ac    BestOf={{0}, 19.313708, NBVILLES};

#elif NBVILLES==30

 Carte carte={{{8,4},{4,1},{1,9},{7,6},{2,1},
               {9,8},{3,5},{4,6},{5,9},{3,9},
               {0,4},{8,5},{5,2},{6,1},{7,8},
               {2,6},{3,6},{6,5},{1,8},{7,1},
               {7,0},{7,3},{0,0},{3,1},{5,1},
               {6,0},{3,3},{1,1},{1,4},{1,6}}, 30};

 Ac    BestOf={{0}, 46.371631, NBVILLES};  /* Meilleur cout ???? */

#else
 Carte carte;             /* Un passage de parametre pas tres joli ! */
#endif                    /*     Voir anticorps.h/anticorps.c        */

/* Et encore des passsages de paramatres pas jolis (NBVILLES) ! */

void visualiserCout(FILE *fd, char *fileName)
{
 fprintf(fd,"plot \"%s\" with linespoints\n",fileName);
 fflush(fd);
}

void ecrireCout(FILE *fdCout, int abscisse, double cout)
{
 if (fdCout==NULL) return;

 fprintf(fdCout,"%d %f\n",abscisse,cout);
 fflush(fdCout);
}

int main(int argc, char** argv)
{
 int nbIndividus=TAILLEPOP;
 int nN=N;
 int dD=D;
 int nbGenerations=NBGENERATIONS;
 int nbGenerationsInjection=NBGENERATIONSINJECTION;

 int nbClones=(nN * nbIndividus)/100;
 int nbNouveaux=(dD * nbIndividus)/100;

 Population p;
 Ac         meilleur, LeMeilleur={{0}, DBL_MAX, NBVILLES};

 int tour=0;


 FILE* fdCout;
 char* fileNameCout="Cout";


 FILE* fdGnuplot;
 FILE* fdGnuplotCout;
 FILE* fdBest;
 char* fileNameBest="Best";
 
 void recupParams(int argc, char **argv,
                 int *nbIndividus, int *nN, int *dD,
                 int *nbGenerations, int *nbGenerationsInjection,
                 int *nbClones, int *nbNouveaux);

 /*************************************************/
 /******** Traitement des arguments ***************/
 /*************************************************/
 if (argc!=1) /* Si il y a des arguments */
 {
  recupParams(argc,argv,&nbIndividus,&nN,&dD,
              &nbGenerations,&nbGenerationsInjection,
              &nbClones,&nbNouveaux);
 }

 fprintf(stderr,"\n*********\n");
 fprintf(stderr,"TAILLEPOP=%d\n",nbIndividus);
 fprintf(stderr,"N=%d => nbClones=%d\n",nN,nbClones);
 fprintf(stderr,"D=%d => nbNouveaux=%d\n",dD,nbNouveaux);
 fprintf(stderr,"NBGENERATIONS=%d\n",nbGenerations);
 fprintf(stderr,"NBGENERATIONSINJECTION=%d\n",nbGenerationsInjection);
 fprintf(stderr,"*********\n\n");

 /****************************************/
 /******** Initialisation ****************/
 /****************************************/

 fdCout=fopen(fileNameCout,"w");  /* Ouverture du fichier pour les couts */
 if (fdCout==NULL) {
   fprintf(stderr,"Probleme sur fopen(\"%s\",\"w\")\n",fileNameCout);
   fprintf(stderr,"=> Arret du programme\n");
   exit(EXIT_FAILURE);
 }

 fdGnuplot=openGnuplot(NULL);                /* pipe + fork pour visu    */
 if (fdGnuplot==NULL) {
   fprintf(stderr,"Probleme sur openGnuplot => Arret du programme\n");
   fclose(fdCout);
   exit(EXIT_FAILURE);
 }

 setRangeGnuplot(fdGnuplot,                  /* Rappel : les coordonnees */
                 -1,COTECARTE,-1,COTECARTE); /* des villes vont          */
                                             /* de 0 a COTECARTE-1       */

 fdGnuplotCout=openGnuplot(NULL);            /* pipe + fork pour visu    */
 if (fdGnuplotCout==NULL) {
   fprintf(stderr,"Probleme sur openGnuplot => Arret du programme\n");
   fclose(fdCout);
   closeGnuplot(fdGnuplot);
   exit(EXIT_FAILURE);
 }

 fdBest=openGnuplot(fileNameBest);           /* Avec gnuplot faire :     */
 if (fdGnuplotCout==NULL) {                  /* gnuplot > load "Best"    */
   fprintf(stderr,"Probleme sur openGnuplot => Arret du programme\n");
   fclose(fdCout);
   closeGnuplot(fdGnuplot);
   closeGnuplot(fdGnuplotCout);
   exit(EXIT_FAILURE);
 }


 initRandom();

#if NBVILLES!=8 && NBVILLES!=16 && NBVILLES!=30
 genereCarte(&carte,COTECARTE);
#endif

 dessineCarte(fdGnuplot,&carte);

 sleep(3);

 printCarte(&carte);

 /*********************************************************/
 /******** Les choses serieuses commencent ****************/
 /*********************************************************/

                                                        /* Parametres:       */
                                                        /* &p(p:population), */
 generePopulation(&p,nbIndividus,nbClones);             /* nbIndividus,      */
                                                        /* nbClones          */



// tester la fonction d mutation ici avant d'aller plus loins

 /*       {
      Ac ac ; 
     genereAc(&ac);
     printAc(&ac);
     muteAc(&ac,1);
     printAc(&ac);
      return 0 ;
        }*/
        
 


 /* Tri d'une Population avec un COUT DECROISSANT =>          */
 /* les moins bons sont a gauche, les meilleurs sont a droite */

 triPopulation(&p);

 tour=0;


 while (tour!=nbGenerations)
 {
  tour++;

  /* Au debut de la boucle, la population doit etre deja triee */

  /* A completer ... */
  
  clonageMeilleurs(&p);   // on clone les meilleurs
  
  mutationClones(&p);     // mutation des clones 

  
  selectionMeilleursEtClonesMutesMethod2(&p); // on compare les clones avec les originaux et on choisit les meilleurs avec la methode du tri
  
  //selectionMeilleursEtClonesMutesMethod2(&p); // on compare les clones avec les originaux et on choisit les meilleurs avec la methode de comparaison 2 à 2 
  
   // triPopulation(&p); // on tri la population pour traiter les moins bon
  
    mutationMoinsBons(&p);   // on applique des mutations sur les clones

  
  // on va traiter les individus les moins bon cette partie de code doit etre evoquée dans certaines reprises et pas à chaque iteration



    if(tour%nbGenerationsInjection == 0 && tour>1) {

  //  triPopulation(&p);
    remplacementMoinsBons(&p,nbNouveaux);
}
  
  

 // appels aux differentes fonctions

  triPopulation(&p);               /* Pour trouver le meilleur, la    */
  meilleur=meilleurIndividu(&p);   /* population doit etre deja triee */


  if (meilleur.cout<LeMeilleur.cout)
  {
   cloneAc(&meilleur,&LeMeilleur);

   dessineParcoursAc(fdGnuplot,&LeMeilleur);
   printCoutAc(&LeMeilleur);

   ecrireCout(fdCout,tour,LeMeilleur.cout);
   visualiserCout(fdGnuplotCout,fileNameCout);
  }

#if NBVILLES==8 || NBVILLES==16 || NBVILLES==30
  if (compareAc(&LeMeilleur,&BestOf)==0) {
     printf("Stop! (meilleure solution trouvee)\n");
     break; 
  }
#endif

 }

 /*********************************************************/
 /***** A la fin de la boucle, on affiche Le Meilleur *****/
 /*********************************************************/

 printf("\n*** Voici la meilleure solution trouvee ***\n");
 dessineParcoursAc(fdGnuplot,&LeMeilleur);
 dessineParcoursAc(fdBest,&LeMeilleur);
 printCoutAc(&LeMeilleur);
 printf("\n*** Avec %s %d %d %d %d %d\n",argv[0],nbIndividus,
                                        nN,dD,nbGenerations,
                                        nbGenerationsInjection);

#if NBVILLES==8 || NBVILLES==16 || NBVILLES==30
 if (compareAc(&LeMeilleur,&BestOf)<=0)
 {
  printf("Meilleure solution obtenue en %d generations\n",tour);
  if (compareAc(&LeMeilleur,&BestOf)!=0)
  {
   printf("ET EN PLUS LE COUT (%f)"
          " EST MEILLEUR QUE LA SOLUTION PREVUE!!\n",LeMeilleur.cout);
  }
 }
 else
 {
  printf("Meilleure solution NON obtenue en %d generations\n",tour);
 }
#endif

#if 1
 sleep(2);                                   /* Dessin des          */
 dessineIndividusPopulation(fdGnuplot,&p,5); /* meilleurs individus */
#endif

 supprimePopulation(&p);

 fclose(fdCout);

 closeGnuplot(fdGnuplot);
 closeGnuplot(fdGnuplotCout);
 closeGnuplot(fdBest);

 exit(EXIT_SUCCESS);
}


/************************************************************/
/******** Lecture et Traitement des arguments ***************/
/************************************************************/

void recupParams(int argc, char **argv,
                 int *nbIndividus, int *nN, int *dD,
                 int *nbGenerations, int *nbGenerationsInjection,
                 int *nbClones, int *nbNouveaux)
{
 int printUsage=0;

 if (argc==6) {
   if (sscanf(argv[1],"%d",nbIndividus)!=1)
   {
    fprintf(stderr,"\n%s n'est pas un entier\n",argv[1]);
    printUsage=1;
   }
   else
   if (sscanf(argv[2],"%d",nN)!=1)
   {
    fprintf(stderr,"\n%s n'est pas un entier\n",argv[2]);
    printUsage=1;
   }
   else
   if (sscanf(argv[3],"%d",dD)!=1)
   {
    fprintf(stderr,"\n%s n'est pas un entier\n",argv[3]);
    printUsage=1;
   }
   else
   if (sscanf(argv[4],"%d",nbGenerations)!=1)
   {
    fprintf(stderr,"\n%s n'est pas un entier\n",argv[4]);
    printUsage=1;
   }
   else
   if (sscanf(argv[5],"%d",nbGenerationsInjection)!=1)
   {
    fprintf(stderr,"\n%s n'est pas un entier\n",argv[5]);
    printUsage=1;
   }
   else
   if (*nN>100)
   {
    fprintf(stderr,"\nN est trop grand, N>100%%\n");
    printUsage=1;
   }
   else
   if (*dD>100)
   {
    fprintf(stderr,"\nD est trop grand, D>100%%\n");
    printUsage=1;
   }
  }
  else printUsage=1;

  if (printUsage)
  {
   fprintf(stderr,"\n");
   fprintf(stderr,"Usage: %s TAILLEPOP N D"
                  " NBGENERATIONS NBGENERATIONSINJECTION\n",argv[0]);
   fprintf(stderr,"\n");
   fprintf(stderr,"Ou N : %% de TAILLEPOP => nbClones\n");
   fprintf(stderr,"Ou D : %% de TAILLEPOP => nbNouveaux\n");
   fprintf(stderr,"\nExemple:\n");
   fprintf(stderr,"%s 200 50 20 300 30\n",argv[0]);
   fprintf(stderr,"=>\n");
   fprintf(stderr,"Taille population: 200\n");
   fprintf(stderr,"Lorsque l'on clone des Ac, nbClones : 100\n");
   fprintf(stderr,"Lorsque l'on injecte/supprime des Ac, nbNouveaux : 40\n");
   fprintf(stderr,"Nb de generations a effecter : 300\n");
   fprintf(stderr,"Nb de generations avant injection/suppression d'Ac : 30\n");
   exit(EXIT_FAILURE);
  }
  else {
         *nbClones=(*nN * *nbIndividus)/100;
         *nbNouveaux=(*dD * *nbIndividus)/100;
  }
}
