#ifndef PARAMS_H
#define PARAMS_H

#define  NBVILLES    30
#define COTECARTE    10

#define  TAILLEPOP   100
#define          N   50   /* Selection: En % de TAILLEPOP (Max 100%!) */
#define          D   20   /* Injection: En % de TAILLEPOP (Max 100%!) */

#define  NBGENERATIONS            100
#define  NBGENERATIONSINJECTION    20

#define   NBCLONES  (N*TAILLEPOP)/100
#define NBNOUVEAUX  (D*TAILLEPOP)/100

/*********************************************************************/
/************ Verification contraintes sur les defines ***************/
/*********************************************************************/


#if NBVILLES>COTECARTE*COTECARTE
#error "Attention: NBVILLES>COTECARTE*COTECARTE"
#endif

#if N>100
#error "Attention: N trop grand"
#endif

#if D>100
#error "Attention: D trop grand"
#endif

#endif /* PARAMS_H */
