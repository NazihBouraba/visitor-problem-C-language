/* random.c : generation de nombres aleatoire */

#include <stdio.h>
#include <stdlib.h>     /* Pour rand, srand                 */
#include <time.h>       /* Pour time (=> Graine pour srand) */

#include "random.h"

/**********************************************************************/
/********* Generation de nombres aleatoires ***************************/
/**********************************************************************/

void initRandom(void)                     /* Initialisation generateur */
{
 static int pasEncore=1;
 if (pasEncore)
 {
  printf("Initialisation du generateur de nombres aleatoires\n");
  srand(time((time_t *)0)); /* Graine (generation de nombres aleatoires) */
  pasEncore=0;
 }
}

int myRandomMinMax(int min, int max)      /* Generation dans [min,max] */
{
 double interval = 0.0; int val;

 if (max<min) { int temp=min; min=max; max=temp; }

 interval = (double)(max-min+1);


 val = min+(int)(interval*rand()/(RAND_MAX));

 while (val<min || val > max)
 {
  val = min+(int)(interval*rand()/(RAND_MAX));
 }

 return val;      /* [min,max] */
}

double myRandom01(void)                   /* Generation dans [0.0,1.0] */
{
 return ((double)rand()/RAND_MAX);                    /* [0.0,1.0] */
}

