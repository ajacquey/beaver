import os, csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve
plt.rcParams['text.usetex'] = True

# Mandel's problem
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 3.2 Mandel's problem
# 
# Time dimension is t = Cv * t / h^2
# Space dimension is x = x / a

class RootFinder:
  def __init__(self, start, stop, step=0.01, root_dtype="float64", xtol=1e-9):

    self.start = start
    self.stop = stop
    self.step = step
    self.xtol = xtol
    self.roots = np.array([], dtype=root_dtype)

  def add_to_roots(self, x):

    if (x < self.start) or (x > self.stop):
      return  # outside range
    if any(abs(self.roots - x) < self.xtol):
      return  # root already found.

    self.roots = np.append(self.roots, x)

  def find(self, f, *args):
    current = self.start

    for x0 in np.arange(self.start, self.stop + self.step, self.step):
      if x0 < current:
        continue
      x = self.find_root(f, x0, *args)
      if x is None:  # no root found.
        continue
      current = x
      self.add_to_roots(x)

    return self.roots

  def find_root(self, f, x0, *args):

    x, _, ier, _ = fsolve(f, x0=x0, args=args, full_output=True, xtol=self.xtol)
    if ier == 1:
      return x[0]
    return None

def func(s): # function to find roots
  # x = a * np.sqrt(s / Cv)
  # return s * (np.sinh(x) / x  - 2.0 * eta * np.cosh(x))
  return np.tan(s) - 2.0 * eta * s

def analyticalSolution_p(x, t):
  # Roots
  xi_list = findRoots(func, 1.0e-04, 40.0, 1.0e-04)

  res = 0.0
  for xi in xi_list:
    # res += 2.0 * eta * np.cos(xi) * (np.cos(xi * x) - np.cos(xi)) / (1.0 - 2.0 * eta * np.square(np.cos(xi))) * np.exp(-np.square(xi) * t)
    res += 2.0 * eta * (np.cos(xi * x) - np.cos(xi)) / (2.0 * eta * xi * np.sin(xi) - (2.0 * eta - 1.0) * np.cos(xi)) * np.exp(-np.square(xi) * t)
  return res / p0

def analyticalSolution_u(t):
  # Roots
  xi_list = findRoots(func, 1.0e-04, 40.0, 1.0e-04)

  res = -alpha * eta * p0 / (2 * (K + G / 3.0)) * b / a
  for xi in xi_list:
    res += b / a * alpha * eta * p0 / (K + G / 3.0) * (1.0 - 2* eta * p0 / q) * np.cos(xi) * np.sin(xi) / (xi - np.cos(xi) * np.sin(xi)) * np.exp(-np.square(xi) * t)
  return res
  
def findRoots(f, x0, xend, step):
  # Defined below
  r = RootFinder(x0, xend, step)
  roots = r.find(f)
  return roots
  # print("Roots: ", roots)

  # # plot results
  # u = np.linspace(x0, xend, num=600)
  # fig, ax = plt.subplots()
  # ax.plot(u, func(u))
  # ax.scatter(roots, f(np.array(roots)), color="r", s=10)
  # ax.grid(color="grey", ls="--", lw=0.5)
  # plt.show()

def numericalSolution(t):
  filebase = "mandel_csv_p_line_pf_"
  if t==0.01:
    filename = filebase + "0047.csv"
  elif t==0.1:
    filename = filebase + "0093.csv"
  elif t==0.5:
    filename = filebase + "0125.csv"
  elif t==1.0:
    filename = filebase + "0139.csv"
  else:
    print('Unknown filename!')
    exit

  y, p = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[2, 1], unpack=True)
  p = np.divide(p, p0)

  return (y, p)

def plot_pressure(ax):
  xa = np.linspace(0.0, 1.0, 100)

  # Values of Cv * t / h**2
  time = [0.01, 0.1, 0.5, 1.0]

  for t in time:
    # Analytical solution
    pa = analyticalSolution_p(xa, t)

    # Numerical solution
    xn, pn = numericalSolution(t)

    # Plot analytical
    ax.plot(xa, pa, lw=1, color="xkcd:blue")

    # Plot numerical
    ax.plot(xn, pn, ls='none', marker='o', ms=2, color='k')

  return None

def plot_consolidation(ax):
  ta = np.logspace(-3, 0, 100)

  # ua = analyticalSolution_u(ta)
  tn, un = np.loadtxt("mandel_csv_t.csv", delimiter=',', skiprows=1, usecols=[0, 4], unpack=True)
  # un = (-un*h - w0) / (wf - w0)

  # Plot analytical
  # ax.plot(ta, ua, lw=1, color='xkcd:blue')
  # Plot numerical
  ax.plot(tn, un, ls='none', marker='o', ms=2, color='k')

  return None

if __name__ == "__main__":
  # INPUT
  a = 1.0
  b = 1.0
  q = 1.0
  phi = 0.1
  k = 1.5
  mu = 1.0
  Kf = 8.0
  K = 1.0
  nu = 0.25
  alpha = 0.6
  # PROPERTIES
  Ks = K / (1.0 - alpha)
  S = phi / Kf + (alpha - phi) / Ks
  eta = (1.0 - nu) / (1.0 - 2.0 * nu) * (alpha**2 + 3.0 * K * S / (2.0 * (1.0 + nu))) / alpha**2
  Cv = k / mu * (3.0 * K * (1.0 - nu) / (1.0 + nu)) / (alpha**2 + 3.0 * K * S * (1.0 - nu) / (1.0 + nu))
  p0 = q * (1.0 - nu) / (2.0 * eta * (1.0 - 2.0 * nu) * alpha)

  print("Reference spatial dimension: ", a)
  print("Reference time dimension: ", Cv / a**2)
  print("Reference pressure dimension: ", p0)
  # print("Initial displacement: ", w0)
  # print("Final displacement: ", wf)

  # Figure pressure
  plt.rc('text', usetex=True)
  fig, ax = plt.subplots()
  fig.set_size_inches(6, 5)
  
  plot_pressure(ax)

  ax.set_xlim(0.0, 1.0)
  ax.set_ylim(0.0, 1.2)
  # ax.grid()
  ax.set_ylabel(r'$\frac{p_{f}}{p_{0}}$', fontsize=16)
  ax.set_xlabel(r'$\frac{x}{a}$', rotation=0, fontsize=16)
  ax.set_title('Mandel\'s problem')

  plt.savefig('mandel_pf.png', dpi=200, bbox_inches='tight') 


  # Figure displacement
  plt.rc('text', usetex=True)
  fig, ax = plt.subplots()
  fig.set_size_inches(10, 5)
  
  plot_consolidation(ax)

  # ax.set_xlim(1.0e-04, 1.0e+01)
  # ax.set_ylim(0.0, 1.0)
  # ax.grid()
  ax.invert_yaxis()
  # ax.set_xscale("log")
  # ax.set_xlabel(r'$\frac{c_{v} t}{h^{2}}$', fontsize=16)
  # ax.set_ylabel(r'$U$', rotation=0, fontsize=16)
  ax.set_title('Mandel\'s problem')

  plt.savefig('mandel_u.png', dpi=200, bbox_inches='tight')