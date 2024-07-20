import os, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
plt.style.use('../../publication.mplstyle')

def analyticalSolution(v):
  return 0.5 * (np.sqrt(np.divide(-2 * v + np.sqrt(4 * v + 1) - 1, v) + 1) + 1) * np.logical_not(v > 0.5 * (1 + np.sqrt(2)))

def numericalSolution(t):
  filebase = "buckley-leverett_csv_line_sw_"
  if t==0.25:
    filename = filebase + "0025.csv"
  elif t==0.5:
    filename = filebase + "0050.csv"
  elif t==0.75:
    filename = filebase + "0075.csv"
  else:
    print("Unknown filename!")
    exit

  x, s = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[2,1], unpack=True)

  return (x, s)

def plot_saturation(ax):
  xa = np.linspace(0, 1, 500)

  # Time values
  time = [0.25, 0.5, 0.75]

  for t in time:
    # Analytical solution
    sa = analyticalSolution(np.divide(xa, t))

    # Numerical solution
    xn, sn = numericalSolution(t)

    # Plot analytical
    ax.plot(xa, sa, color="xkcd:red")
    # Plot numerical
    ln = ax.scatter(xn, sn, c="k", marker="o", linewidths=0.0)

  # Legend
  labels = ["Analytical", "Numerical"]
  ax.legend([Line2D([0], [0], color="xkcd:red"), ln], labels, loc="best")

  return None

if __name__ == "__main__":
  # INPUT
  si = 0.0
  sr = 1.0

  # Figure saturation
  fig, ax = plt.subplots(figsize=(5.0, 2.5))

  plot_saturation(ax)

  ax.set_xlim(0, 1)
  ax.set_ylim(0, 1)
  # ax.set_xscale("log")
  # ax.set_xlabel(r"$\frac{c_{v} t}{a^{2}}$")
  # ax.set_ylabel(r"$\frac{p_{c}}{p_{0}}$", rotation=0, labelpad=10)
  plt.show()
