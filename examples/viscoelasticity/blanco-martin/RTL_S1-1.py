import numpy as np
import matplotlib.pyplot as plt

plt.style.use('publication.mplstyle')
plt.rcParams['text.usetex'] = False  # Ensure LaTeX is not used for rendering text

def read_csv(filename):
    # Read data
    time, EpsAx = np.loadtxt(filename, delimiter=',', skiprows=1, usecols=[0, 4], unpack=True)
    return time, EpsAx

def plot_figure():
    fig, ax = plt.subplots()
    ax.set_title('Axial_strain (-) vs time (d)')  # Use Unicode directly
    
# Reading data
    time, EpsAx = read_csv("RTL_S1-1.csv")   
# Plot AE data
    ax.plot(time, EpsAx, color="k", label="EpsAx")  
# Additional plot settings
    ax.set_xlabel('Time (days)')
    ax.set_ylabel('EpsAx (-)')
  #  ax.set_yscale('log')  # Set y-axis to log base 10
    ax.legend()  
# Axes limit
#    ax.set_xlim(4, 8)
# Save figure
    fig.savefig("RTL_S1-1.pdf", format="pdf", dpi=300, bbox_inches="tight")

if __name__ == '__main__':
    plot_figure()
