import numpy as np
import matplotlib.pyplot as plt

plt.style.use('publication.mplstyle')
plt.rcParams['text.usetex'] = False  # Ensure LaTeX is not used for rendering text

def read_experimental_data():
    filename = "Fig8_Bas_Bottom.csv"
    gamma, q = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[4, 9], unpack=True, encoding='ISO-8859-1')
    return gamma, q

def read_simulation_data():
    filename = "BVRTL2020salt5triaxialtest3_csv.csv"  # Replace with your actual filename
    gamma, q = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[10, 11], unpack=True)
    return gamma, q

def plot_figure():
    fig, ax = plt.subplots()
    ax.set_title('q vs $\gamma$ for bottom experiment')

    # Read data
    gamma_exp, q_exp = read_experimental_data()
    gamma_sim, q_sim = read_simulation_data()

    # Plot experimental data
    ax.plot(gamma_exp, q_exp, color="k", label="Experimental q")
    # Plot simulation data
    ax.plot(gamma_sim, q_sim, color="r", label="Simulation q")

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
