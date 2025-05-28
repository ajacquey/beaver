import numpy as np
import matplotlib.pyplot as plt
import csv

def calculate_gamma_vp(t, A_r=1, T_r=289, T=289, q=2, A_2=1, alpha=0.1, beta=1.48):
    """Calculate the analytical solution for gamma_vp"""
    exponent = A_r * ((1 / T_r) - (1 / T))
    numerator = alpha * np.exp(exponent)
    denominator = alpha
    first_term = (numerator / denominator) ** alpha
    second_term = (q / A_2) ** beta
    return 1e-6* first_term * second_term * (t ** alpha)

# Time range from 0 to 1 with step 0.1
t_values = np.arange(0, 1.0, 0.001)

# Calculate gamma_vp for each time point
gamma_vp_values = [calculate_gamma_vp(t) for t in t_values]

# --- Save to CSV ---
with open('Lemaitre.csv', 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['Time (t)', 'Gamma_vp'])
    for t, gamma in zip(t_values, gamma_vp_values):
        writer.writerow([t, gamma])

# --- Create Plot ---
plt.figure(figsize=(8, 5))
plt.plot(t_values, gamma_vp_values, 'b-', linewidth=2)
plt.legend([r'$\gamma_{vp}$'], loc='upper left', fontsize=12)
plt.title(r'$\gamma_{vp}$ vs Time (t)', fontsize=14)
plt.xlabel('Time (t)', fontsize=12)
plt.ylabel(r'$\gamma_{vp}$', fontsize=12)
plt.grid(True, linestyle='--', alpha=0.7)
plt.xticks(np.arange(0, 1.1, 0.1))
plt.yticks(np.arange(0, 3.1, 0.1))
plt.tight_layout()

# Save and show plot
plt.savefig('gamma_vp_plot.png', dpi=300)
plt.show()

print("Results saved to:")
print("- CSV file: 'Lemaitre.csv'")
print("- Plot: 'gamma_vp_plot.png'")