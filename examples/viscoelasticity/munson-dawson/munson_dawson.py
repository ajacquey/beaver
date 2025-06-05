import os, csv
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

# Munson-Dawson creep test
# Units
# Stress in MPa
# Strain dimensionless
# Time in days

# Model parameters
gamma_R0 = 0.0
A1, A2 = 0.3, 0.5
n1, n2 = 3.0, 4.0  
A, B = 0.5, 0.05
n, m = 5.0, 5.0
t_switch = 15.0
q1, q2 = 10.0, 5.0

def analyticalSolution(t_array):
    gamma_R_results = []
    gamma_R_bar_results = []
    q_results = []
    scaling_factor = 1e-6
    
    # Find gamma_R at switch point for second segment
    gamma_R_at_switch = gamma_R0
    
    for i, t in enumerate(t_array):
        # Get q value based on time
        if t <= t_switch:
            q = q1
        else:
            q = q2
        
        # Calculate gamma_R_bar
        gamma_R_bar = np.power(q / A1, n1)
        
        # Determine initial conditions and time reference for current segment
        if t <= t_switch:
            # First segment
            gamma_R_current = gamma_R0
            t0 = 0
            q_val = q1
        else:
            # Second segment - use gamma_R value at switch point
            if len(gamma_R_results) > 0:
                # Find the last point before or at switch
                switch_indices = [idx for idx, time_val in enumerate(t_array[:i]) if time_val <= t_switch]
                if switch_indices:
                    gamma_R_at_switch = gamma_R_results[switch_indices[-1]] / scaling_factor  # Unscale for calculation
                else:
                    gamma_R_at_switch = gamma_R0
            else:
                gamma_R_at_switch = gamma_R0
            
            gamma_R_current = gamma_R_at_switch
            t0 = t_switch
            q_val = q2
        
        # Determine regime and calculate coefficients
        if gamma_R_current <= gamma_R_bar:
            k = n
            U = A * np.power(1 - gamma_R_current/gamma_R_bar, n-1)
        else:
            k = m
            U = B * np.power(gamma_R_current/gamma_R_bar - 1, m-1)

        # Calculate Z
        Z = np.power(A1, n1) / np.power(A2, n2) * np.power(q_val, n2 - n1)

        # Calculate V
        dt = max(t - t0, 0)
        V = max((k - 1) * U * Z * dt, 0)
        
        # Calculate xi with numerical stability
        if abs(k - 1) < 1e-10:
            xi = 1 - np.exp(-V) if V < 700 else 1.0  # Prevent overflow
        else:
            if V > 0:
                xi = 1 - np.power(1 / (1 + V), 1/(k-1))
            else:
                xi = 0
        
        # Calculate gamma_R
        gamma_R = gamma_R_current + (gamma_R_bar - gamma_R_current) * xi
        
        # Store scaled results
        gamma_R_results.append(gamma_R * scaling_factor)
        gamma_R_bar_results.append(gamma_R_bar * scaling_factor)
        q_results.append(q)
    
    return gamma_R_results, gamma_R_bar_results, q_results

def numericalSolution():
    filename = "munson_dawson_csv.csv"
    
    t, gamma_R, gamma_R_bar = np.loadtxt(filename, delimiter=',', skiprows=1, 
                                        usecols=[0, 3, 2], unpack=True)
    
    return t, gamma_R, gamma_R_bar

def plot_munson_dawson(ax):
    """
    Plot Munson-Dawson analytical and numerical solutions
    """
    # Time array for analytical solution
    ta = np.linspace(0.0, 30.0, 1000)
    
    # Analytical solution
    gamma_R_a, gamma_R_bar_a, q_a = analyticalSolution(ta)
    
    # Numerical solution
    tn, gamma_R_n, gamma_R_bar_n = numericalSolution()

    # Plot analytical solutions
    ax.plot(ta, gamma_R_a, color="red", linewidth=2, label="Analytical")
    # ax.plot(ta, gamma_R_bar_a, color="blue", linestyle="--", linewidth=2, label="γ̄_R (Analytical)")
    
    # Plot numerical solutions
    ax.scatter(tn, gamma_R_n, c="k", marker="o", s=20, linewidths=0.0, label="Numerical")
    # ax.scatter(tn, gamma_R_bar_n, c="gray", marker="s", s=20, linewidths=0.0, label="γ̄_R (Numerical)")

    # Add vertical line at stress switch
    ax.axvline(x=t_switch, color='k', linestyle=':', alpha=0.7, label=r"Stress switch ($t = {}$ days)".format(t_switch))

    # Legend
    ax.legend(loc="best")

    # Additional plot settings
    ax.set_ylabel(r"Creep strain, $\gamma_{R}$ ($\mu m / m$)")
    ax.set_xlabel(r"Time, $t$ (days)")
    # ax.grid(True, alpha=0.3)

if __name__ == '__main__':
    # Figure setup
    fig, ax = plt.subplots()
    
    plot_munson_dawson(ax)

    ax.set_xlim(0, 30.0)
    ax.set_ylim(0, None)  # Auto-scale y-axis

    fig.savefig("../../../doc/content/media/munson_dawson_strain.png", format="PNG", dpi=300, bbox_inches="tight")