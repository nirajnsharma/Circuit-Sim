#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*
 * Program to simulate the node voltages of a simple RC circuit. 
 * The RC circuit can be defined by a pair of ODEs in the time domain. The
 * dependent variable being the two node voltages. 
 * Solving these two ODEs using backward euler approximation
 *
 * Notes:
 * Right now we are making a guess for the time constant, tau. Later we will
 * find this out by finding the eigenvalues of the 
 * Backward Euler is preferred over Forward, despite have the same order as BE
 * does not have any problems with stability
 */

void fn_invert_2x2 (double *i, double *o) {
   double iMatrix [2][2];
   double oMatrix [2][2];
   int r, c;
   double determinant;

   // Unpacking a matrix
   for (r=0; r<2; r++) {
      for (c=0; c<2; c++) {
         // In general i[(r*N)+c]
         iMatrix [r][c] = i[(r*2)+c];
      }
   }

   determinant = iMatrix [0][0] * iMatrix [1][1];
   determinant = determinant - (iMatrix [0][1] * iMatrix [1][0]);

   oMatrix[0][0] =  iMatrix[1][1]/determinant;
   oMatrix[0][1] = (-1.0 * iMatrix[0][1])/determinant;
   oMatrix[1][0] = (-1.0 * iMatrix[1][0])/determinant;
   oMatrix[1][1] =  iMatrix[0][0]/determinant;

   // Packing a matrix
   for (r=0; r<2; r++) {
      for (c=0; c<2; c++) {
         o[(r*2)+c] = oMatrix [r][c];
      }
   }
}


void fn_multiply_2x1 (double *i, double *jVec, double *kVec) {
   double iMatrix [2][2];
   int r, c;

   // Unpacking a matrix
   for (r=0; r<2; r++) {
      for (c=0; c<2; c++) {
         // In general i[(r*N)+c]
         iMatrix [r][c] = i[(r*2)+c];
      }
   }

   // The actual inner product
   for (r=0; r<2; r++) {
      for (c=0; c<2; c++) {
         if (c == 0) {
            kVec [r] = iMatrix[r][c] * jVec [c];
         } else {
            kVec [r] += iMatrix[r][c] * jVec [c];
         }
      }
   }
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
   double       vs_nn   = 0.0;         // The voltage source, vs(t)
   double       g1      = 1.0/r1;
   double       g2      = 1.0/r2;

   double       A [2][2];              // The A matrix to solve the two linear
                                       // equations in v1_nn and v2_nn
   double       b [2];                 // The LHS of the Ax=b equation

   double       *a_vec, *t_vec, *o_vec;// Vector forms to send matrices to fns

   int          iter    = 0;           // To keep track of the iterations
   int          r, c    = 0;

   // To write out the data to be plotted
   FILE *fPlot;
   fPlot = fopen ("rc-be.dat", "w");

   a_vec = (double *) malloc (4 * sizeof (double));
   t_vec = (double *) malloc (4 * sizeof (double));
   o_vec = (double *) malloc (2 * sizeof (double));

   fprintf (fPlot, "# t  x  \n");

   // The loop implementing the backward euler approximation
   do {
      // Compute vs_n
      vs_nn = vm * sin (omega * (t_n+h));
      printf ("Iteration: %d\n", iter);
      fprintf (fPlot, "%13.6e %13.6e %13.6e\n", t_n, v1_n, v2_n);

      // Compose the A matrix to solve the set of linear equations for
      // backward-euler
      A [0][0] = 1.0 + ((h/c1)*(g1+g2));  // argument for v1_nn (node 1)
      A [0][1] = -(h/c1)*g2;              // argument for v2_nn (node 1)
      A [1][0] = -(h/c2)*g2;              // argument for v1_nn (node 2)
      A [1][1] = 1.0 + ((h/c2)*g2);       // argument for v2_nn (node 2)

      // Pack the a-matrix
      for (r=0; r<2; r++) {
         for (c=0; c<2; c++) {
            a_vec [(r*2)+c] = A [r][c];
         }
      }

      b [0] = v1_n + ((h/c1)*g1*vs_nn);
      b [1] = v2_n;

      fn_invert_2x2 (a_vec, t_vec);
      fn_multiply_2x1 (t_vec, b, o_vec);

      // Forward for the next iteration
      t_n += h;
      v1_n = o_vec[0];
      v2_n = o_vec[1];
      iter++;
   } while (t_n < t_end);

   // Print results from the last iteration
   printf ("Iteration: %d\n", iter);
   fprintf (fPlot, "%13.6e %13.6e %13.6e\n", t_n, v1_n, v2_n);

   fclose (fPlot);

   return 0;
}
