#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*
 * Program to solve the differential equation dx/dt = 5*(t**2)*(x**2)
 * using the trapezoidal approximation technique
 * The trapezoidal method needs us to solve for x(n+1) by finding the root
 * of a non-linear equation in x(n+1) (henceforth xnn). This is done by the
 * Newton-Raphson method for each iteration of time step (the independent
 * variable). So total iterations = (time-iterations * NR iterations) 
 *
 * Notes:
 * The f and df need to be consistent. If we are factoring out a term in the f,
 * we need to do it consistently in both f and df, and not selectively as that
 * will change the ratio f/df. You either leave it on both, or remove on both.
 */

// Flag to print stuff for debug. Set to 1 for debug messages
int debug = 0;

// df/dxnn = 5.h.(tnn^2)xnn - 1
double fn_df (
        double h     // Time-delta
      , double t_nn  // t_n+1
      , double x_nn) // the variable we are solving for
{
   double t_nn_sq = t_nn * t_nn;
   double df = 10.0 * h * t_nn_sq * x_nn;
          df = df - 2.0;
   return (df);
}

double fn_f (
        double h     // Time-delta
      , double t_n   // t_n -- current time
      , double t_nn  // t_n+1
      , double x_n   // x_n
      , double x_nn  // x_n+1
      )
{
   double t_n_sq = t_n * t_n;
   double x_n_sq = x_n * x_n;
   double t_nn_sq = t_nn * t_nn;
   double x_nn_sq = x_nn * x_nn;

   double f = 5.0 * h * t_nn_sq * x_nn_sq;
          f = f - (2.0 * x_nn);
          f = f + (5.0 * h * t_n_sq * x_n_sq) + (2.0 * x_n);

   return (f);
}

double fn_newtonRaphson (
        double h            // Time-delta
      , double t_n          // t_n -- current time
      , double x_n          // x_n -- current x
      , int    iter         // number of iterations within which we expect convergence
      , double tolerance    // error tolerance from root when we can stop
      , double reallySmall  // If the df becomes reallySmall, something is wrong
      )
{
   double t_nn = t_n + h;   // t_nn initialized to current-time + h
   double x_nn = x_n;       // starting guess that x_nn is really close to x_n
   int    i    = 0;         // iterator

   double f, df;

   for (i = 0; i < iter; i++) {
      f = fn_f (h, t_n, t_nn, x_n, x_nn);
      if (fabs (f) <= tolerance) {
         // If we are within tolerance, distance-wise from the actual root
         if (debug == 1)
            printf ("NR [%d]: f = %13.6e\n", i, f);
         break;             // Close enough to the root
      }

      df = fn_df (h, t_nn, x_nn);
      if (fabs (df) < reallySmall) {
         printf ("Something really bad happened ... df too small. Quitting\n");
         exit (1);
      } 

      // The equation whose root we are trying to find is an equation in x_nn.
      // So, every iteration update x_nn
      x_nn = x_nn - (f/df);

      if (debug == 1) {
         printf (
               "NR [%d]: f = %13.6e, df = %13.6e, xn = %13.6e, xnn = %13.6e \n"
               , i, f, df, x_n, x_nn);
      }
   }

   // Did we exit the loop due to the break or because we ran out of iterations?
   if (fabs (f) <= tolerance) {
      return (x_nn);
   }
   
   else {
      printf ("NR did not converge in %d iterations\n", iter);
      printf ("Increase iteration count. Quitting \n");
      exit (1);
   }
}


int main (void) {
  double x_n      = -1.0;  // x(0) -- initial condition
  double h        =  0.01; // time increment
  double t_n      =  0.0;  // Initial time
  double t_f      =  5.0;  // Final time

  double x_nn;             // next x_n

  int    iteration= 0;     // To keep track of the main loop

  // To write out the data to be plotted
  FILE *fPlot;
  fPlot = fopen ("trapezoidal.dat", "w");
  fprintf (fPlot, "#     t        x  \n");

  // The loop implementing the trapezoidal approximation
  do {
    fprintf (fPlot, "%13.6e %13.6e\n", t_n, x_n);

    // Compute the x_n+1 by solving the non-linear equation
    x_nn = fn_newtonRaphson (h, t_n, x_n, 10, 1.0e-8, 1.0e-10);

    // Moving forward -- increment time, move x_n
    t_n = t_n + h;
    x_n = x_nn;
    
    iteration++;
    printf ("Iteration: %d\n", iteration);
  } while (t_n < t_f);

  fclose (fPlot);

  return 0;
}
