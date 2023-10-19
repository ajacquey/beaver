import os, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
plt.style.use('../../publication.mplstyle')

# Stress build-up in a Lubby2's viscoelastic medium
#
# Time dimension is t = G * t / etaK
# Strain dimension is eqv_strain = eqv_strain / (eqv_stress / G)
m_1 = 0.327
m_2 = 0.267
m_G = 0.254
se = np.sqrt(3.0)
etaK_etaM = 0.1

def analyticalSolution(t):
  return np.exp(m_G * se) / np.sqrt(3.0) * (1.0 - np.exp(-np.exp(-se * (m_G - m_2)) * t) + etaK_etaM * np.exp(-se * (m_G - m_1)) * t)
  
def numericalSolution():
  filename = "lubby2_csv.csv"

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
  ax.set_ylim(0.0, 2.0)
  ax.set_xlabel(r"$\overline{t} = \frac{G_{K0}}{\eta_{K0}}t$")
  ax.set_ylabel(r"$\overline{\epsilon} = \frac{G_{K0}}{\sigma_{0}} \epsilon$")

  # plt.show()
  fig.savefig("../../../doc/content/media/lubby2_strain.png", format="PNG", dpi=300, bbox_inches="tight")
