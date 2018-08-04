# A script to plot the following plots:
# forwardEuler.dat
# backwardEuler.dat
# trapezoidal.dat
# the analytic function
#
# Visually see how much closer the trapezoidal is to the analytic

load 'forwardEuler.p'
replot 'backwardEuler.dat' with lines
replot 'trapezoidal.dat' with lines
analytic(x) = -3/(5*(x*x*x) + 3)
replot analytic(x)

