import os, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
plt.style.use('../../publication.mplstyle')

# Stress build-up in a linear Maxwell viscoelastic medium
#
# See Gerya and Yuen (2007)
# Section 3.1
# 
# Time dimension is t = G * t / eta
# Stress dimension is tau = tau / (eta * gamma_dot)

def analyticalSolution(t):
  return 1.0 - np.exp(-t)
  
def numericalSolution():
  filename = "linear_maxwell_csv.csv"

  t, tau = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 1], unpack=True)

  return (t, tau)

def plot_stress(ax):
  ta = np.linspace(0.0, 10.0, 100)

  # Analytical solution
  sa = analyticalSolution(ta)
  # Numerical solution
  tn, sn = numericalSolution()

  # Plot analytical
  ax.plot(ta, sa, color="xkcd:red", label="Analytical")
  # Plot numerical
  ax.scatter(tn, sn, c="k", marker="o", linewidths=0.0, label="Numerical")

  # Legend
  ax.legend(loc="best")

  return None

if __name__ == "__main__":

  # Figure stress
  fig, ax = plt.subplots()
  
  plot_stress(ax)

  ax.set_xlim(0.0, 10.0)
  ax.set_ylim(0.0, 1.2)
  ax.set_xlabel(r"$\overline{t} = \frac{G}{\eta} t$")
  ax.set_ylabel(r"$\overline{\tau} = \frac{\tau}{\eta \dot{\gamma}}$")

  # plt.show()
  fig.savefig("../../../doc/content/media/linear_maxwell_tau.png", format="PNG", dpi=300, bbox_inches="tight")
