import os, csv
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('../../publication.mplstyle')

def plot_experimental_data(ax):
  # Read data from file
  t, stress, strain = np.loadtxt("S1-1.txt", skiprows=7, usecols=[0, 1, 3], unpack=True)

  # ax.plot(t, stress, color="k")
  ax[0].plot(t, strain / 1.0e+04, color="k")
  ax[1].plot(t, stress, color="k", label=r"Experimental data")

def plot_simulation_data(ax):
  # Read data from file
  t, stress, strain = np.loadtxt("blanco-martin-RTL-2024_csv.csv", delimiter=',', skiprows=1, usecols=[0, 6, 5], unpack=True)

  # ax.plot(t, -stress, color="xkcd:red")
  ax[0].plot(t, -strain * 1.0e+02, color="xkcd:red")
  ax[1].plot(t, -stress, color="xkcd:red", label=r"Numerical fit")

if __name__ == "__main__":

  # Figure stress
  fig, axes = plt.subplots(2, 1, figsize=(3.25, 5), sharex=True)
  # plt.subplots_adjust(hspace=0.12)
  
  plot_experimental_data(axes)
  plot_simulation_data(axes)

  axes[1].set_xlim(0, 200)
  axes[0].set_ylim(0, 8)
  axes[1].set_ylim(0, 30)
  axes[1].set_xlabel(r"Time, (days)")
  axes[0].set_ylabel(r"Axial strain, $\varepsilon_{\text{ax}}$ (\%)")
  axes[1].set_ylabel(r"Axial stress, $\sigma_{\text{ax}}$ (MPa)")
  axes[0].set_title(r"Modified RTL2020 model (Blanco-Martìn et al., 2024)")
  axes[1].legend(loc="lower right")

  # plt.show()
  fig.savefig("blanco-martin-RTL-2024.png", format="PNG", dpi=300, bbox_inches="tight")
