import os, csv
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('../publication.mplstyle')

# Fluid-induced aseismic slip on a 1D fault 
#
# See Robert C. Viesca "Self-similar fault slip in response to fluid injection" 2023

if __name__ == "__main__":
    T = 0.4
    lmbda = 1.0252441
    mu_p = 5.33e+01

    # Analytical solution from Viesca (2023)
    x = np.linspace(0.0, 1.0, 21)
    slip = np.array([0.26854750, 0.26495909, 0.25676095, 0.24543311, 0.23179150, 0.21642674, 0.19981014, 0.18233746, 0.16435133, 0.14615421, 0.12801685, 0.11018427, 0.092880849, 0.076314980, 0.060684332, 0.046182579, 0.033009402, 0.021387701, 0.011599155, 0.0040812723, 0.0])
    # Scale to the simulations
    x = lmbda * np.concatenate((-np.flipud(x), x[1:]))
    slip = lmbda / mu_p * np.concatenate((np.flipud(slip), slip[1:]))

    # Read Julia results
    x_ddm, slip_ddm = np.loadtxt("outputs/julia/constant_friction_injection_2D_T_0.4/constant_friction_injection_2D_T_0.4_500.csv", delimiter=',', skiprows=1, usecols=[0, 3], unpack=True)
    
    # Read Beaver results
    x_fem, slip_fem = np.loadtxt("outputs/constant_friction_injection_2D_T_0.4/constant_friction_injection_2D_T_0.4_fault_pp_0500.csv", delimiter=',', skiprows=1, usecols=[6, 4], unpack=True)
    # Figure
    fig, ax = plt.subplots()
    ax.plot(x, slip, "-k")
    ax.plot(x_ddm, slip_ddm, color="xkcd:red")
    ax.plot(x_fem, slip_fem, color="xkcd:blue")
    ax.set_xlim(-2, +2)
    ax.set_ylim(0, 1.25 * np.amax(slip))
    plt.show()

    print("Slip max analytical : \n\t-> ", np.amax(slip), "\n")
    print("Slip max DDM : \n\t-> ", np.amax(slip_ddm), "\t ", 100 * (np.abs(np.amax(slip) - np.amax(slip_ddm))) / np.amax(slip), "%\n")
    print("Slip max FEM : \n\t-> ", np.amax(slip_fem), "\t ", 100 * (np.abs(np.amax(slip) - np.amax(slip_fem))) / np.amax(slip), "%\n")



