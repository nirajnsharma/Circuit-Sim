Contains various "recipes" explaining different computation techniques for certain scientific compuing problems. They are in a mixture of C, Octave and Python. Educational.

# Circuit Sim
Contains common techniques to simulate some standard circuits along with tools
implemented along the way. Mostly in C and a couple in Octave

# Matrix Computation
Elementary functions in Octave. 

# blas-lapack
Implementation of problems which use Ax=b solvers using BLAS and LAPACK
functions

# optimization
Contains implementation of optimization techniques like particle-swarm

Several of the programs are written with noweb, the included Makefile generates
the latex and .m or .c source.

To compile the noweb source (assuming that you have noweb installed):

  make DOC=<doc-base-name> ROOT=<code-root>

The doc-base-name is the name of the file you want to compile, without the
extension. The ROOT refers to the first code snippet in the file.

For instance,

  make DOC=householder ROOT=tb

Include in the optimzation, blas-lapack and Matrix-Computation directories are
wrapper scripts which can be used to build-all programs.
