#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*
 * Program to simulate the node voltages of a simple RC circuit. 
 * The RC circuit can be defined by a pair of ODEs in the time domain. The
 * dependent variable being the two node voltages. 
 * Solving these two ODEs using forward euler approximation
 *
 * Notes:
 * Right now we are making a guess for the time constant, tau. Later we will
 * find this out by finding the eigenvalues of the 
 * Forward Euler has a tendency to become unstable when the time-step is larger
 * than twice the smaller of the time-constants in the circuit
 */

// df/dt = 1/c1 ((vs-v1)/r1 + (v2-v1)/r2)
double fn_df_v1 (
        double vs
      , double v1
      , double v2
      , double c1
      , double r1
      , double r2
      ) {
   double df_v1 = 1.0/c1;
          df_v1 = df_v1 * (((vs - v1)/r1) - ((v2 - v1)/r2));
  return (df_v1);
}

double fn_df_v2 (
        double v1
      , double v2
      , double c2
      , double r2
      ) {
   double df_v2 = -1.0/c2;
          df_v2 = df_v2 * ((v2 - v1)/r2);
   return (df_v2);
}

int main (void) {
   const double c1      = 1.0e-6;      // Given value of C1 = 1uF
   const double c2      = 1.0e-6;      // Given value of C2 = 1uF
   const double r1      = 1.0e+3;      // Given value of R1 = 1kO
   const double r2      = 1.0e+3;      // Given value of R2 = 1kO
   const double vm      = 1.0;         // Given value of Vs' amplitude (1V)
   const double vf      = 1.0e+3;      // Given value of Vs' frequency (1kHz)
   const double pi      = acos (-1.0); // pi to the accuracy permitted
   const double omega   = 2 * pi * vf;
   const double tau     = 1.0e-3;      // A *guess* of the time constant

   const double h       = tau * 0.01;  // Should be h < 2*min(tau1,tau2)
                                       // Otherwise F-euler becomes unstable
   const double t_end   = tau * 15;    // Beyond tau, we expect convergence, 
                                       // so no point simulating endlessly.
                                       // How much beyond tau?
   double       t_n     = 0.0;         // Starting from time zero
   double       v1_n    = 0.0;         // Assuming C1 is initially discharged
   double       v2_n    = 0.0;         // Assuming C2 is initially discharged
   double       v1_nn   = 0.0;         // v1_n+1
   double       v2_nn   = 0.0;         // v2_n+1
   double       vs_n    = 0.0;         // The voltage source, vs(t)

   int          iter    = 0;           // To keep track of the iterations

   // To write out the data to be plotted
   FILE *fPlot;
   fPlot = fopen ("rc-fe.dat", "w");

   fprintf (fPlot, "# t  x  \n");

   // The loop implementing the forward euler approximation
   do {
      // Compute vs_n
      vs_n = vm * sin (omega * t_n);
      printf ("Iteration: %d\n", iter);
      fprintf (fPlot, "%13.6e %13.6e %13.6e\n", t_n, v1_n, v2_n);
      v1_nn = (h * fn_df_v1 (vs_n, v1_n, v2_n, c1, r1, r2)) + v1_n;
      v2_nn = (h * fn_df_v2 (v1_n, v2_n, c2, r2)) + v2_n;

      // Forward for the next iteration
      t_n += h;
      v1_n = v1_nn;
      v2_n = v2_nn;
      iter++;
   } while (t_n < t_end);

   // Print results from the last iteration
   printf ("Iteration: %d\n", iter);
   fprintf (fPlot, "%13.6e %13.6e %13.6e\n", t_n, v1_n, v2_n);

   fclose (fPlot);

   return 0;
}
