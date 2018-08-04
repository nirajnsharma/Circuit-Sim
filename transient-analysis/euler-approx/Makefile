.PHONY : all
all : forwardEuler trapezoidal backwardEuler

forwardEuler: forwardEuler.c
	gcc forwardEuler.c -lm -o forwardEuler
trapezoidal: trapezoidal.c
	gcc trapezoidal.c -lm -o trapezoidal
backwardEuler: backwardEuler.c
	gcc backwardEuler.c -lm -o backwardEuler

.PHONY: clean
clean:
	rm -f forwardEuler trapezoidal backwardEuler
