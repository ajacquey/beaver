import os, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
plt.style.use('../../publication.mplstyle')

# Terzaghi's problem of consolodation of a soil layer
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 2.2 Terzaghi's problem
# 
# Time dimension is t = Cv * t / h^2
# Space dimension is z = z / h

def analyticalSolution_p(z, t):
  res = 0.0
  for k in range(1,500):
    res += 4.0 / np.pi * np.power(-1.0, k - 1.0) / (2.0 * k - 1.0) * np.cos((2.0 * k -1.0) * np.pi / 2.0 * z) * np.exp(-np.power(2.0 * k - 1.0, 2.0) * np.power(np.pi, 2.0) / 4.0 * t)
  return res

def analyticalSolution_u(t):
  res = 1.0
  for k in range(1,500):
    res -= 8.0 / np.square(np.pi) / np.square(2.0 * k - 1.0) * np.exp(-np.square(2.0 * k - 1.0) * np.square(np.pi) / 4.0 * t)
  return res
  
def numericalSolution(t):
  filebase = "terzaghi_csv_p_line_pf_"
  if t==0.001:
    filename = filebase + "0047.csv"
  elif t==0.005:
    filename = filebase + "0079.csv"
  elif t==0.01:
    filename = filebase + "0093.csv"
  elif t==0.05:
    filename = filebase + "0125.csv"
  elif t==0.1:
    filename = filebase + "0139.csv"
  elif t==0.2:
    filename = filebase + "0153.csv"
  elif t==0.5:
    filename = filebase + "0172.csv"
  elif t==1.0:
    filename = filebase + "0186.csv"
  else:
    print("Unknown filename!")
    exit

  y, p = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[3, 1], unpack=True)
  p = np.divide(p, p0)

  return (y, p)

def plot_pressure(ax):
  za = np.linspace(0.0, 1.0, 100)

  # Values of Cv * t / h**2
  time = [0.001, 0.005, 0.01, 0.05, 0.1, 0.2, 0.5, 1.0]

  for t in time:
    # Analytical solution
    pa = analyticalSolution_p(za, t)
    # Numerical solution
    zn, pn = numericalSolution(t)

    # Plot analytical
    ax.plot(pa, za, color="xkcd:red")
    # Plot numerical
    ln = ax.scatter(pn, zn, c="k", marker="o", linewidths=0.0)
  
  # Legend
  labels = ["Analytical", "Numerical"]
  ax.legend([Line2D([0], [0], color="xkcd:red"), ln], labels, loc="best")

  return None

def plot_consolidation(ax):
  ta = np.logspace(-4, 1, 100)

  ua = analyticalSolution_u(ta)
  tn, un = np.loadtxt("terzaghi_csv_u.csv", delimiter=',', skiprows=1, usecols=[0, 1], unpack=True)
  un = (-un*h - w0) / (wf - w0)

  # Plot analytical
  ax.plot(ta, ua, color="xkcd:red", label="Analytical")
  # Plot numerical
  ax.scatter(tn, un, c="k", marker="o", linewidths=0.0, label="Numerical")

  # Legend
  ax.legend(loc="best")

  return None

if __name__ == "__main__":
  h = 10.0
  q = 1.0
  phi = 0.1
  k = 1.5
  mu = 1.0
  Kf = 8.0
  Ks = 10.0
  K = 4.0
  G = 3.0
  alpha = 1.0 - K / Ks # 0.6
  S = phi / Kf + (alpha - phi) / Ks
  mv = 1.0 / (K + 4.0 / 3.0 * G)
  Cv = k / (mu * (S + alpha**2 * mv))
  p0 = alpha * mv / (S + alpha**2 *mv) * q
  w0 = mv * h * q * S / (S + alpha**2 * mv)
  wf = mv * h * q
  print("Reference spatial dimension: ", h)
  print("Reference time dimension: ", Cv / h**2)
  print("Reference pressure dimension: ", p0)
  print("Initial displacement: ", w0)
  print("Final displacement: ", wf)

  # Figure pressure
  fig, ax = plt.subplots()
  
  plot_pressure(ax)

  ax.text(0.15, 0.1, r"$1$", horizontalalignment="center", verticalalignment="center")
  ax.text(0.42, 0.15, r"$0.5$", horizontalalignment="center", verticalalignment="center")
  ax.text(0.8, 0.2, r"$0.2$", horizontalalignment="center", verticalalignment="center")
  ax.text(0.85, 0.45, r"$0.1$", horizontalalignment="center", verticalalignment="center")
  ax.text(0.85, 0.65, r"$0.05$", horizontalalignment="center", verticalalignment="center")
  ax.text(0.7, 0.8, r"$0.01$", horizontalalignment="center", verticalalignment="center")
  ax.text(0.65, 0.92, r"$0.005$", horizontalalignment="center", verticalalignment="center")
  ax.text(0.5, 1.05, r"$\frac{c_{v} t}{h^{2}} = 0.001$", horizontalalignment="center", verticalalignment="center")
  
  ax.set_xlim(0.0, 1.1)
  ax.set_ylim(0.0, 1.2)
  ax.set_xlabel(r"$\frac{p}{p_{0}}$")
  ax.set_ylabel(r"$\frac{z}{h}$", rotation=0,  labelpad=10)

  fig.savefig("../../../doc/content/media/terzaghi_pf.png", format="PNG", dpi=300, bbox_inches="tight")

  # Figure displacement
  fig, ax = plt.subplots(figsize=(5.0, 2.5))
  
  plot_consolidation(ax)

  ax.set_xlim(1.0e-04, 1.0e+01)
  ax.set_ylim(0.0, 1.0)
  ax.invert_yaxis()
  ax.set_xscale("log")
  ax.set_xlabel(r"$\frac{c_{v} t}{h^{2}}$")
  ax.set_ylabel(r"$U$", rotation=0, labelpad=10)

  fig.savefig("../../../doc/content/media/terzaghi_u.png", format="PNG", dpi=300, bbox_inches="tight")