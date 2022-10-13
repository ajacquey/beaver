import os, csv
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams['text.usetex'] = True

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
    print('Unknown filename!')
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
    ax.plot(pa, za, lw=1, color='xkcd:blue')
    # Plot numerical
    ax.plot(pn, zn, ls='none', marker='o', ms=2, color='k')

  return None

def plot_consolidation(ax):
  ta = np.logspace(-4, 1, 100)

  ua = analyticalSolution_u(ta)
  tn, un = np.loadtxt("terzaghi_csv_u.csv", delimiter=',', skiprows=1, usecols=[0, 1], unpack=True)
  un = (-un*h - w0) / (wf - w0)

  # Plot analytical
  ax.plot(ta, ua, lw=1, color='xkcd:blue')
  # Plot numerical
  ax.plot(tn, un, ls='none', marker='o', ms=2, color='k')

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
  plt.rc('text', usetex=True)
  fig, ax = plt.subplots()
  fig.set_size_inches(6, 5)
  
  plot_pressure(ax)

  ax.set_xlim(0.0, 1.1)
  ax.set_ylim(0.0, 1.0)
  ax.set_xlabel(r'$\frac{p_{f}}{p_{0}}$', fontsize=16)
  ax.set_ylabel(r'$\frac{z}{h}$', rotation=0, fontsize=16, labelpad=20)
  ax.set_title('Terzaghi\'s consolidation problem')

  plt.savefig('terzaghi_pf.png', dpi=200, bbox_inches='tight') 

  # Figure displacement
  plt.rc('text', usetex=True)
  fig, ax = plt.subplots()
  fig.set_size_inches(10, 5)
  
  plot_consolidation(ax)

  ax.set_xlim(1.0e-04, 1.0e+01)
  ax.set_ylim(0.0, 1.0)
  ax.invert_yaxis()
  ax.set_xscale("log")
  ax.set_xlabel(r'$\frac{c_{v} t}{h^{2}}$', fontsize=16)
  ax.set_ylabel(r'$U$', rotation=0, fontsize=16, labelpad=20)
  ax.set_title('Terzaghi\'s consolidation problem')

  plt.savefig('terzaghi_u.png', dpi=200, bbox_inches='tight')