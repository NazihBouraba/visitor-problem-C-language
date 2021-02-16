                                 /* gnuplot.h */

#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <stdio.h>

/**********************************************************************/
/********* Affichage avec gnuplot et un tube **************************/
/**********************************************************************/

typedef struct { float x,y; } PointGnuplot;

extern FILE* openGnuplot(char *fileName);
extern void  closeGnuplot(FILE* flot);

extern void  setAutoscaleGnuplot(FILE* flot);

extern void  setRangeGnuplot(FILE* flot,
                             float xmin, float xmax, float ymin, float ymax);

extern void  beginPointsToGnuplot(FILE* flot,
                                  char *style);   /* "lines" , "linespoint" ,
                                                     "points"
                                                  */

extern void  pointsToGnuplot(FILE* flot,
                             PointGnuplot tabPoint[], int nbPoints);

extern void  endPointsToGnuplot(FILE* flot);

extern void  vectorGnuplot(FILE* flot,
                           float x1, float y1, float x2, float y2);

#endif /* GNUPLOT_H */
