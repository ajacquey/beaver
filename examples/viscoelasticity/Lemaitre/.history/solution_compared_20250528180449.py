import numpy as np
import matplotlib.pyplot as plt

# plt.style.use('publication.mplstyle')  # Comment out if style file doesn't exist
plt.rcParams['text.usetex'] = False  # Ensure LaTeX is not used for rendering text

def read_analytical_data():
    filename = "Lemaitre.csv"
    try:
        t, gamma_vp = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 2], unpack=True)
        return t, gamma_vp
    except FileNotFoundError:
        print(f"File {filename} not found!")
        return None, None
    except IndexError:
        print(f"Column index error in {filename}. Check if column 2 exists.")
        return None, None

def read_simulation_data():
    filename = "Lemaitre_csv.csv"  # Replace with your actual filename
    try:
        t, gamma_vp = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 3], unpack=True)
        return t, gamma_vp
    except FileNotFoundError:
        print(f"File {filename} not found!")
        return None, None
    except IndexError:
        print(f"Column index error in {filename}. Check if column 3 exists.")
        return None, None

def plot_figure():
    fig, ax = plt.subplots(figsize=(8, 6))
    ax.set_title(r't vs $\gamma_{vp}$')  # Fixed LaTeX syntax

    # Read data
    t_exp, gamma_vp_exp = read_analytical_data()
    t_sim, gamma_vp_sim = read_simulation_data()

    # Check if data was loaded successfully
    if t_exp is None or gamma_vp_exp is None:
        print("Failed to load analytical data")
        return
    if t_sim is None or gamma_vp_sim is None:
        print("Failed to load simulation data")
        return

    # Plot experimental data
    ax.plot(t_exp, gamma_vp_exp, color="k", label="Experimental q", linewidth=2)
    # Plot simulation data
    ax.plot(t_sim, gamma_vp_sim, color="r", label="Simulation q", linewidth=2)

    # Set x-axis limit
    ax.set_xlim(None, 4.69)

    # Additional plot settings
    ax.set_xlabel(r'Shear Strain ($\gamma$)/%')
    ax.set_ylabel('Differential Stress (Q-P)/MPa')
    # ax.set_yscale('log')  # Uncomment if log scale is desired
    ax.legend()
    ax.grid(True, alpha=0.3)  # Add grid for better readability

    # Save figure
    fig.savefig("q_gamma_compared_bottom.pdf", format="pdf", dpi=300, bbox_inches="tight")
    plt.show()  # Display the plot

if __name__ == '__main__':
    plot_figure()