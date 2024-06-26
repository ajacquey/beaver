import os, csv
import numpy as np
from scipy import special
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
plt.style.use('../../publication.mplstyle')

# Advection-diffusion equation

def analyticalSolution(x, t):
  # return (c0 + ci) / 2 - (ci - c0) / 2 * special.erf(np.divide(x - u * t, np.sqrt(4 * D * t)))
  return (ci - c0) / 2 * (special.erfc(np.divide(x - u * t, np.sqrt(4 * D * t))) + np.multiply(np.exp(u * x / D), special.erfc(np.divide(x + u * t, np.sqrt(4 * D * t)))))

def numericalSolution(t):
  filebase = "outputs/advection-diffusion_line_c_"
  if t==0.2:
    filename = filebase + "0200.csv"
  elif t==0.4:
    filename = filebase + "0400.csv"
  elif t==0.6:
    filename = filebase + "0600.csv"
  else:
    print("Unknown filename!")
    exit

  x, c = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[2, 0], unpack=True)

  return (x, c)

def plot_concentration(ax):
  xa = np.linspace(0.0, 1.0, 500)

  # Values of time
  time = [0.2, 0.4, 0.6]

  for t in time:
    # Analytical solution
    ca = analyticalSolution(xa, t)
    # Numerical solution
    xn, cn = numericalSolution(t)

    # Plot analytical
    ax.plot(xa, ca, color="xkcd:red")
    # # Plot numerical
    ln = ax.scatter(xn, cn, c="k", marker="o", linewidths=0.0)

  # Legend
  labels = ["Analytical", "Numerical"]
  ax.legend([Line2D([0], [0], color="xkcd:red"), ln], labels, loc="best")

  return None

if __name__ == "__main__":
  # INPUT
  ci = 1
  c0 = 0
  u = 1
  L = 1
  D = 1.0e-03

  # Figure pressure
  fig, ax = plt.subplots()

  plot_concentration(ax)

  # ax.text(0.15, 0.1, r"$1$", horizontalalignment="center", verticalalignment="center")
  # ax.text(0.42, 0.15, r"$0.5$", horizontalalignment="center", verticalalignment="center")
  # ax.text(0.8, 0.2, r"$0.2$", horizontalalignment="center", verticalalignment="center")
  # ax.text(0.85, 0.45, r"$0.1$", horizontalalignment="center", verticalalignment="center")
  # ax.text(0.85, 0.65, r"$0.05$", horizontalalignment="center", verticalalignment="center")
  # ax.text(0.7, 0.8, r"$0.01$", horizontalalignment="center", verticalalignment="center")
  # ax.text(0.65, 0.92, r"$0.005$", horizontalalignment="center", verticalalignment="center")
  # ax.text(0.5, 1.05, r"$\frac{c_{v} t}{h^{2}} = 0.001$", horizontalalignment="center", verticalalignment="center")

  ax.set_xlim(0.0, 1.0)
  ax.set_ylim(0.0, 1.2)
  ax.set_xlabel(r"Time $t$")
  ax.set_ylabel(r"Concentration $c$")

  fig.savefig("../../../doc/content/media/advection-diffusion.png", format="PNG", dpi=300, bbox_inches="tight")
  # plt.show()
  # # Figure displacement
  # fig, ax = plt.subplots(figsize=(5.0, 2.5))

  # plot_consolidation(ax)

  # ax.set_xlim(1.0e-04, 1.0e+01)
  # ax.set_ylim(0.0, 1.0)
  # ax.invert_yaxis()
  # ax.set_xscale("log")
  # ax.set_xlabel(r"$\frac{c_{v} t}{h^{2}}$")
  # ax.set_ylabel(r"$U$", rotation=0, labelpad=10)

  # fig.savefig("../../../doc/content/media/terzaghi_u.png", format="PNG", dpi=300, bbox_inches="tight")
