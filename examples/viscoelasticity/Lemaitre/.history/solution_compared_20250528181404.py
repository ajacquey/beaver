import numpy as np
import matplotlib.pyplot as plt

# plt.style.use('publication.mplstyle')  # Comment out if style file doesn't exist
plt.rcParams['text.usetex'] = False  # Ensure LaTeX is not used for rendering text

def read_analytical_data():
    filename = "Lemaitre.csv"
    t, gamma_vp = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 2], unpack=True)
    return t, gamma_vp

def read_simulation_data():
    filename = "Lemaitre_csv.csv"  # Replace with your actual filename
    t, gamma_vp = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 3], unpack=True)
    return t, gamma_vp

def plot_figure():
    fig, ax = plt.subplots(figsize=(8, 6))
    ax.set_title(r't vs $\gamma_{vp}$')  # Fixed LaTeX syntax

    # Read data
    t_analytical, gamma_vp_exp = read_analytical_data()
    t_sim, gamma_vp_sim = read_simulation_data()

    # Plot experimental data
    ax.plot(t_analytical, gamma_vp_exp, color="k", label="Analytical _ $\gamma_{vp}$", linewidth=2)
    # Plot simulation data
    ax.plot(t_sim, gamma_vp_sim, color="r", label="Simulation _ $\gamma_{vp}$", linewidth=2)

    # Set x-axis limit
    ax.set_xlim(None, 1.0)

    # Additional plot settings
    ax.set_ylabel(r'Shear Strain ($\gamma_{vp}$)/%')
    ax.set_xlabel('Time (days)')
    # ax.set_yscale('log')  # Uncomment if log scale is desired
    ax.legend()
    ax.grid(True, alpha=0.3)  # Add grid for better readability

    # Save figure
    fig.savefig("simulation_vs_analytical_sol.pdf", format="pdf", dpi=300, bbox_inches="tight")
    plt.show()  # Display the plot

if __name__ == '__main__':
    plot_figure()