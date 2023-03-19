import os, csv
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('../../publication.mplstyle')

# Cryer's problem
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 3.3 Cryer's problem
# 
# Time dimension is t = Cv * t / r^2
# Space dimension is x = x / r

def func(s): # function to find roots
  return (1.0 - eta * s**2 / 2.0) * np.tan(s) - s

def analyticalSolution_p(t):
  # Roots
  xi_list = findRoot()

  res = 0.0
  for xi in xi_list:
    res += eta * (np.sin(xi) - xi) / ((eta - 1.0) * np.sin(xi) + eta * xi * np.cos(xi) / 2.0) * np.exp(-xi**2 * t)
  return res

def numericalSolution_p():
  filename = "cryer_csv_t.csv"
  
  t, p = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 1], unpack=True)
  p = np.divide(p, p0)

  return (t, p)

def findRoot():
  eps = 1.0e-08
  N = 200
  res = np.zeros(N)

  for i in range(N):
    a1 = eps + (2* i + 1) * np.pi / 2.0
    da = np.pi
    b1 = func(a1)
    for j in range(5):
      for k in range(N):
        a2 = a1 + da / N
        b2 = func(a2)
        if (b2 * b1 < 0.0):
          da = a2 - a1
          break
        else:
          a1 = a2
          b1 = b2
    res[i] = (a1 + a2) / 2.0
  
  return res

def plot_pressure_center(ax):
  # Values of Cv * t / h**2
  ta = np.logspace(-4, 1, 500)

  # Analytical solution
  pa = analyticalSolution_p(ta)

  # Numerical solution
  xn, pn = numericalSolution_p()

  # Plot analytical
  ax.plot(ta, pa, color="xkcd:red", label="Analytical")

  # Plot numerical
  ax.scatter(xn, pn, c="k", marker="o", linewidths=0.0, label="Numerical")
  
  # Legend
  ax.legend(loc="best")

  return None

if __name__ == "__main__":
  # INPUT
  r = 1.0
  q = 1.0
  phi = 0.1
  k = 1.5
  mu = 1.0
  Kf = 8.0
  K = 1.0
  nu = 0.25
  alpha = 0.6
  # PROPERTIES
  Cs = (1.0 - alpha) / K
  S = phi / Kf + (alpha - phi) * Cs
  KS = K * S
  eta = (1.0 - nu) / (1.0 - 2.0 * nu) * (1.0 + KS / alpha**2)
  p0 = q / (alpha * eta) * (1.0 - nu) / (1.0 - 2.0 * nu)
  Cv = k / mu * (3.0 * K * (1.0 - nu) / (1.0 + nu)) / (alpha**2 + 3.0 * KS * (1.0 - nu) / (1.0 + nu))

  print("Reference spatial dimension: ", r)
  print("Reference time dimension: ", Cv / r**2)
  print("Reference pressure dimension: ", p0)

  # Figure pressure at center
  fig, ax = plt.subplots(figsize=(5.0, 2.5))
  
  plot_pressure_center(ax)

  ax.set_xlim(1.0e-04, 10.0)
  ax.set_ylim(0.0, 1.5)
  ax.set_xscale("log")
  ax.set_xlabel(r"$\frac{c_{v} t}{a^{2}}$")
  ax.set_ylabel(r"$\frac{p_{c}}{p_{0}}$", rotation=0, labelpad=10)

  fig.savefig("../../../doc/content/media/cryer_pf.png", format="PNG", dpi=300, bbox_inches="tight")