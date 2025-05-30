import os, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
plt.style.use('../../publication.mplstyle')

# Creep test using the Lemaitre model
# Units
# Stress in MPa
# Strain in %
# Time in days
q = 2.0
A_2 = 1.0
alpha = 0.1
beta = 1.48

def analyticalSolution(t):
  return 1.0e-02 * np.power(q / A_2, beta) * np.power(t, alpha)

def numericalSolution():
  filename = "lemaitre_csv.csv"

  t, e = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 3], unpack=True)

  return (t, e * 1.0e+04)

def plot_lemaitre(ax):
  ta = np.linspace(0.0, 1.0, 500)
    
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

  # Additional plot settings
  ax.set_ylabel(r"Shear creep strain, $\gamma_{vp}$ (\%)")
  ax.set_xlabel(r"Time, $t$ (days)")
  # ax.set_yscale('log')  # Uncomment if log scale is desired

if __name__ == '__main__':
  # Figure stress
  fig, ax = plt.subplots()
    
  plot_lemaitre(ax)

  ax.set_xlim(-0.05, 1.0)
  ax.set_ylim(0.0, 0.03)

  # plt.show()
  fig.savefig("../../../doc/content/media/lemaitre_strain.png", format="PNG", dpi=300, bbox_inches="tight")