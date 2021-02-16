#include <stdio.h>        /* gnuplot.c */
#include <sys/types.h>
#include <unistd.h>       /* Pour exit, fork   */
#include <stdlib.h>       /* Pour EXIT_FAILURE */
#include <errno.h>        /* Pour errno        */
#include <string.h>       /* Pour strcmp       */

#define PipeEtFork

#include "gnuplot.h"


/**********************************************************************/
/********* Affichage avec gnuplot et un tube **************************/
/**********************************************************************/

/* typedef struct { float x,y; } PointGnuplot; */

static int stderrDejaUtilise = 0;

FILE* openGnuplot(char *fileName)
{
 FILE *fd;

 if (fileName!=NULL && strcmp(fileName,"")!=0) {

  fd=fopen(fileName,"w");
  if (fd==NULL) printf("Attention:openGnuplot, probleme avec fopen\n");
  return fd; 

 }

#ifdef PipeEtFork

 {

  int tube[2];
  int ok;
  pid_t fils;

  ok = pipe(tube);
  if (ok==-1) {
    printf("Attention:openGnuplot, probleme avec pipe\n");
    exit(EXIT_FAILURE);
  }

  do {
      fils=fork();
  } while ((fils==-1) && (errno==EAGAIN));
  if (fils==-1) {
    printf("Attention:openGnuplot, probleme avec fork\n");
    exit(EXIT_FAILURE);
  }

  if (fils==0) {
                 /* Le fils */

   dup2(tube[0],STDIN_FILENO);   /* tube[0] => Entree standard */
   close(tube[0]);      /* Il est donc maintenant possible de fermer tube[0] */
   close(tube[1]);      /* ESSENTIEL : pour la fin du tube                   */

   execlp("gnuplot","gnuplot","-noraise","-persist",NULL);

   printf("Attention:initGnuplot, probleme avec execlp\n");
   exit(EXIT_FAILURE);
  }

  /* Le processus principal continue */

  close(tube[0]);

#if 1

  fd = fdopen(tube[1],"a");
  if (fd==NULL) {
    perror("fdopen");
    printf("Attention:openGnuplot, probleme avec fdopen\n");
    close(tube[1]);
    exit(EXIT_FAILURE);
  }
  /* Et on ne fait pas : close(tube[1]); !!! */

#else
                              /* Si on veut vraiment travailler avec stderr !*/
  dup2(tube[1],STDERR_FILENO);           /* stderr <- tube[1] */
  close(tube[1]);
  fd = stderr;

#endif

 }

#else  /* Avec un lancement : $pg $* | gnuplot -persist */

 {

    /* 
       Ici, On inverse stdout et stderr de facon a envoyer
       stderr dans le pipe |
       => Mis a part dans les fonctions decrites ici,
          il ne faut pas utiliser stderr
    */

    /* Si on a besoin du script lancement : $ lancement pg param1 param2 ...

       #!/bin/bash

       if test $# -eq 0
       then pg=ais
       else pg=$1
            shift
       fi

       export PATH=.:$PATH
 
       if test -x $pg
       then
       $pg $* | gnuplot -persist
       else
       echo "$pg n'est pas un programme executable"
       fi

    */

   int temp=dup(STDOUT_FILENO);         /*   temp <- stdout */      
   close(STDOUT_FILENO);              
   dup2(STDERR_FILENO,STDOUT_FILENO);   /* stdout <- stderr */
   close(STDERR_FILENO);
   dup2(temp,STDERR_FILENO);            /* stderr <- temp   */
   close(temp);

   fd = stderr;       /* Et l'ecriture se fera sur stderr ! */

 }
#endif

 if (fd==stderr)
 {
  if (stderrDejaUtilise)
  {
   printf("\n");
   printf("*********************************************************\n");
   printf("* Attention:openGnuplot, stderr utilise plusieus fois ! *\n");
   printf("*********************************************************\n");
  }
  else { stderrDejaUtilise=1;
         printf("\n");
         printf("************************************************\n");
         printf("* stderr ne doit pas etre utilise dans le code *\n");
         printf("************************************************\n");
         printf("\n");
  }
 }  

 return fd;
}

void closeGnuplot(FILE* flot)
{
 fclose(flot);
}


void setAutoscaleGnuplot(FILE* flot)
{
 fprintf(flot,"set autoscale\n");
 fflush(flot);
}

void setRangeGnuplot(FILE* flot,
                     float xmin, float xmax, float ymin, float ymax)
{
 fprintf(flot,"set xrange [%f:%f]\n",xmin,xmax);
 fprintf(flot,"set yrange [%f:%f]\n",ymin,ymax);
 fflush(flot);
}


void beginPointsToGnuplot(FILE* flot,
                          char *style)  /* "lines" , "linespoint" , "points" */
{
 fprintf(flot,"plot '-' with %s\n",style);
 fflush(flot);
}


void pointsToGnuplot(FILE* flot, PointGnuplot tabPoint[], int nbPoints)
{
 int i=0;
 
 for(i=0;i<nbPoints;i++)
 {
  fprintf(flot,"%f %f\n",tabPoint[i].x,tabPoint[i].y);
 }
 fprintf(flot,"\n");             /* Pour marquer la fin du polygone */
 fflush(flot);
}

void endPointsToGnuplot(FILE* flot)
{
 fprintf(flot,"e\n");
 fflush(flot);
}

void vectorGnuplot(FILE* flot, float x1, float y1, float x2, float y2)
{
 fprintf(flot,"%f %f\n",x1,y1);
 fprintf(flot,"%f %f\n",x2,y2);
 fprintf(flot,"\n");            /* Pour marquer la fin du vecteur */
 fflush(flot);
}
