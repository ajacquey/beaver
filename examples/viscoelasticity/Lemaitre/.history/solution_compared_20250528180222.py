import numpy as np
import matplotlib.pyplot as plt

plt.style.use('publication.mplstyle')
plt.rcParams['text.usetex'] = False  # Ensure LaTeX is not used for rendering text

def read_analytical_data():
    filename = "Lemaitre.csv"
    t, gamma_vp = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 3], unpack=True)
    return t, gamma_vp

def read_simulation_data():
    filename = "Lemaitre_csv.csv"  # Replace with your actual filename
    t, gamma_vp = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 3], unpack=True)
    return t, gamma_vp

def plot_figure():
    fig, ax = plt.subplots()
    ax.set_title('t vs $\gamma_vp')

    # Read data
    t, gamma_vp_exp = read_analytical_data()
    t, gamma_vp_sim = read_simulation_data()

    # Plot experimental data
    ax.plot(t, gamma_vp_exp, color="k", label="Experimental q")
    # Plot simulation data
    ax.plot(t, gamma_vp_sim, color="r", label="Simulation q")

    # Set x-axis limit
    ax.set_xlim(None,4.69)

    # Additional plot settings
    ax.set_xlabel(r'shear Strain ($\gamma$)/%')
    ax.set_ylabel('Differential Stress (Q-P)/MPa')
    # ax.set_yscale('log')  # Uncomment if log scale is desired
    ax.legend()

    # Save figure
    fig.savefig("q_gamma_compared_bottom.pdf", format="pdf", dpi=300, bbox_inches="tight")

if __name__ == '__main__':
    plot_figure()
