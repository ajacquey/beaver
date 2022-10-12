import os, csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve
plt.rcParams['text.usetex'] = True

# Mandel's problem
#
# See Arnold Verruijt "Theory and Problems of Poroelasticity" 2015
# Section 3.2 Mandel's problem
# Cheng and Detournay (1988), A direct boundary element method for plane strain poroelasticity
# 
# Time dimension is t = Cv * t / a^2
# Space dimension is x = x / a

def func(s): # function to find roots
  return np.tan(s) - (1.0 - nu) / (nuu - nu) * s

def analyticalSolution_p(x, t):
  # Roots
  xi_list = findRoot()

  res = 0.0
  for xi in xi_list:
    res += 2.0 / a * (np.sin(xi) * np.cos(xi * x) / (xi - np.sin(xi) * np.cos(xi)) - np.sin(xi) * np.cos(xi) / (xi - np.sin(xi) * np.cos(xi))) * np.exp(-np.square(xi) * t)
  return res

def analyticalSolution_u(t):
  # Roots
  xi_list = findRoot()
  
  res = wf
  for xi in xi_list:
    res -= 2.0  * w0 * np.sin(xi) * np.cos(xi) / (xi - np.sin(xi) * np.cos(xi)) * np.exp(-np.square(xi) * t)
  return (res - w0) / (wf - w0)

def findRoot():
  eps = 1.0e-08
  N = 200
  res = np.zeros(N)

  for i in range(N):
    a1 = i * np.pi + np.pi / 4.0
    a2 = a1 + np.pi / 2.0 - eps
    for j in range(N):
      y1 = func(a1)
      y2 = func(a2)
      am = (a1 + a2) / 2.0
      ym = func(am)
      if (ym * y1 > 0.0):
        a1 = am
      else:
        a2 = am
      if (np.abs(y2) < eps):
        am = a2
    res[i] = am
  
  return res

def numericalSolution_x(t):
  filebase = "mandel_csv_p_line_pf_"
  if t==0.01:
    filename = filebase + "0047.csv"
  elif t==0.1:
    filename = filebase + "0093.csv"
  elif t==0.5:
    filename = filebase + "0125.csv"
  elif t==1.0:
    filename = filebase + "0139.csv"
  elif t==2.0:
    filename = filebase + "0153.csv"
  else:
    print('Unknown filename!')
    exit

  y, p = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[2, 1], unpack=True)
  p = np.divide(p, p0)

  return (y, p)

def numericalSolution_t(x):
  filename = "mandel_csv_t.csv"
  if x==0.0:
    t, p = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 1], unpack=True)
  elif x==0.25:
    t, p = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 2], unpack=True)
  elif x==0.5:
    t, p = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 3], unpack=True)
  elif x==0.75:
    t, p = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 4], unpack=True)
  else:
    print('Unknown filename!')
    exit

  p = np.divide(p, p0)

  return (t, p)

def plot_pressure_x(ax):
  xa = np.linspace(0.0, a, 500)

  # Values of Cv * t / h**2
  time = [0.01, 0.1, 0.5, 1.0, 2.0]

  for t in time:
    # Analytical solution
    pa = analyticalSolution_p(xa, t)

    # Numerical solution
    xn, pn = numericalSolution_x(t)

    # Plot analytical
    ax.plot(xa, pa, lw=1, color="xkcd:blue")

    # Plot numerical
    ax.plot(xn, pn, ls='none', marker='o', ms=2, color='k')

  return None

def plot_pressure_t(ax):
  ta = np.logspace(-3, 1, 500)

  # Values of x / a
  x_arr = [0.0, 0.25, 0.5, 0.75]

  for x in x_arr:
    # Analytical solution
    pa = analyticalSolution_p(x, ta)

    # Numerical solution
    tn, pn = numericalSolution_t(x)

    # Plot analytical
    ax.plot(ta, pa, lw=1, color="xkcd:blue")

    # Plot numerical
    ax.plot(tn, pn, ls='none', marker='o', ms=2, color='k')

  return None

def plot_consolidation(ax):
  ta = np.logspace(-3, 1, 500)

  ua = analyticalSolution_u(ta)
  tn, un = np.loadtxt("mandel_csv_t.csv", delimiter=',', skiprows=1, usecols=[0, 5], unpack=True)
  un = (un - w0) / (wf - w0)

  # Plot analytical
  ax.plot(ta, ua, lw=1, color='xkcd:blue')
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
  Cs = (1.0 - alpha) / K
  S = phi / Kf + (alpha - phi) * Cs
  KS = K * S
  Ku = (alpha**2 + KS) / S
  KSG = 3.0 * KS / (2.0 * (1.0 + nu))
  B = alpha / (alpha**2 + KS)
  nuu = 0.5 * (Ku - K * (1.0 - 2.0 * nu) / (1.0 + nu)) / (Ku + K * (1.0 - 2.0 * nu) / (2.0 * (1.0 + nu)))
  Cv = k / mu * B**2 * K * (1.0 - 2.0 * nu) * (1.0 - nu) * (1.0 + nuu)**2 / (3.0 * (1.0 + nu) * (1.0 - nuu) * (nuu - nu))
  p0 = q * B * (1.0 + nuu) / (3.0 * a)
  wf = - q * (1.0 - nu**2) / (3.0 * K * (1.0 - 2.0 * nu)) * b / a
  w0 = - q * (1.0 - nuu) * (1.0 + nu) / (3.0 * K * (1.0 - 2.0 * nu)) * b / a

  print("Reference spatial dimension: ", a)
  print("Reference time dimension: ", Cv / a**2)
  print("Reference pressure dimension: ", p0)

  # Figure pressure - x
  plt.rc('text', usetex=True)
  fig, ax = plt.subplots()
  fig.set_size_inches(6, 5)
  
  plot_pressure_x(ax)

  ax.set_xlim(0.0, 1.0)
  ax.set_ylim(0.0, 1.2)
  ax.set_ylabel(r'$\frac{p_{f}}{p_{0}}$', rotation=0, fontsize=16)
  ax.set_xlabel(r'$\frac{x}{a}$', fontsize=16)
  ax.set_title('Mandel\'s problem')

  plt.savefig('mandel_pf_x.png', dpi=200, bbox_inches='tight') 


  # Figure pressure - t
  plt.rc('text', usetex=True)
  fig, ax = plt.subplots()
  fig.set_size_inches(6, 5)
  
  plot_pressure_t(ax)

  ax.set_xlim(1.0e-03, 1.0e+01)
  ax.set_ylim(0.0, 1.2)
  ax.set_xscale("log")
  ax.set_xlabel(r'$\frac{c_{v} t}{a^{2}}$', fontsize=16)
  ax.set_ylabel(r'$\frac{p_{f}}{p_{0}}$', rotation=0, fontsize=16)
  
  ax.set_title('Mandel\'s problem')

  plt.savefig('mandel_pf_t.png', dpi=200, bbox_inches='tight')

  # Figure consolidation
  plt.rc('text', usetex=True)
  fig, ax = plt.subplots()
  fig.set_size_inches(10, 5)
  
  plot_consolidation(ax)

  ax.set_xlim(1.0e-03, 1.0e+01)
  ax.set_ylim(0.0, 1.0)
  ax.invert_yaxis()
  ax.set_xscale("log")
  ax.set_xlabel(r'$\frac{c_{v} t}{a^{2}}$', fontsize=16)
  ax.set_ylabel(r'$U$', rotation=0, fontsize=16)
  ax.set_title('Mandel\'s problem')

  plt.savefig('mandel_u.png', dpi=200, bbox_inches='tight')
