#! /bin/bash

make -f Makefile.nw DOC=newtonRaphsonMulti ROOT=includes
make -f Makefile.nw DOC=poisson ROOT=includes
make -f Makefile.nw DOC=polyFit ROOT=includes
