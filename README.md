Contains various "recipes" explaining different computation techniques for certain scientific compuing problems. They are in a mixture of C, Octave and Python. Educational.

# Circuit Sim
Contains common techniques to simulate some standard circuits along with tools
implemented along the way. Mostly in C and a couple in Octave

# Matrix Computation
Elementary functions in Octave. Several of the Octave scripts are written with
noweb, the included Makefile generates the latex and .m source. To compile the
noweb source (assuming that you have noweb installed):

make DOC=<doc-base-name> ROOT=<code-root>

The doc-base-name is the name of the file you want to compile, without the
extension. The ROOT refers to the first code snippet in the file. For instance,

make DOC=householder ROOT=tb
