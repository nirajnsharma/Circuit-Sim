#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*
 * Program to solve the differential equation dx/dt = 5*(t**2)*(x**2)
 * using the Euler approximation techniques
 */

// df/dt = 5(t**2)(x**2)
double df (double t, double x) {
  return (5 * t * t * x * x);
}

int main (void) {
  double x_n      = -1.0; // x(0)
  double h        =  0.01;// time increment
  double t_n      =  0.0;
  double t_f      =  5.0;

  // To write out the data to be plotted
  FILE *fPlot;
  fPlot = fopen ("1-1.dat", "w");

  fprintf (fPlot, "# t  x  \n");

  // The loop implementing the forward euler approximation
  do {
    fprintf (fPlot, "%13.6e %13.6e\n", t_n, x_n);
    x_n += (df (t_n, x_n) * h);
    t_n += h;
  } while (t_n < t_f);

  fclose (fPlot);

  return 0;
}
