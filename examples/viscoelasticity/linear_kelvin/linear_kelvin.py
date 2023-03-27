import os, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
plt.style.use('../../publication.mplstyle')

# Stress build-up in a linear Kelvin viscoelastic medium
#
# Time dimension is t = G * t / eta
# Strain dimension is eqv_strain = eqv_strain / (eqv_stress / G)

def analyticalSolution(t):
  return 1.0 / np.sqrt(3.0) * (1.0 - np.exp(-t))
  
def numericalSolution():
  filename = "linear_kelvin_csv.csv"

  t, e = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 2], unpack=True)

  return (t, e)

def plot_stress(ax):
  ta = np.linspace(0.0, 10.0, 100)

  # Analytical solution
  ea = analyticalSolution(ta)
  # Numerical solution
  tn, en = numericalSolution()

  # Plot analytical
  ax.plot(ta, ea, color="xkcd:red", label="Analytical")
  # Plot numerical
  ax.scatter(tn, en, c="k", marker="o", linewidths=0.0, label="Numerical")

  # Legend
  ax.legend(loc="best")

  return None

if __name__ == "__main__":

  # Figure stress
  fig, ax = plt.subplots()
  
  plot_stress(ax)

  ax.set_xlim(0.0, 10.0)
  ax.set_ylim(0.0, 0.75)
  ax.set_xlabel(r"$\frac{G t}{\eta}$")
  ax.set_ylabel(r"$\frac{G \epsilon}{\sigma_{e}}$", rotation=0,  labelpad=10)

  # plt.show()
  fig.savefig("../../../doc/content/media/linear_kelvin_strain.png", format="PNG", dpi=300, bbox_inches="tight")
