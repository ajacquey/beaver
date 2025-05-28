import numpy as np
import matplotlib.pyplot as plt
import csv

def calculate_gamma_vp(t, A_r=1, T_r=289, T=289, q=2, A_2=1, alpha=0.1, beta=1.48):
    """
    Calculate the analytical solution for gamma_vp
    
    Parameters:
    -----------
    t : float or array
        Time values
    A_r : float, default=1
        Arrhenius constant
    T_r : float, default=289
        Reference temperature
    T : float, default=289
        Current temperature
    q : float, default=2
        Flow rate parameter
    A_2 : float, default=1
        Area parameter
    alpha : float, default=0.1
        Alpha exponent
    beta : float, default=1.48
        Beta exponent
    
    Returns:
    --------
    float or array
        Calculated gamma_vp values
    """
    exponent = A_r * ((1 / T_r) - (1 / T))
    numerator = alpha * np.exp(exponent)
    denominator = alpha
    first_term = (numerator / denominator) ** alpha
    second_term = (q / A_2) ** beta
    return first_term * second_term * (t ** alpha)

def save_to_csv(t_values, gamma_vp_values, filename="gamma_vp_results.csv"):
    """Save results to CSV file"""
    with open(filename, 'w', newline='', encoding='utf-8') as csvfile:
        writer = csv.writer(csvfile)
        # Write header
        writer.writerow(["Time (t)", "Gamma_vp"])
        # Write data
        for t, gamma in zip(t_values, gamma_vp_values):
            writer.writerow([t, gamma])
    print(f"Data successfully saved to {filename}")

def create_plot(t_values, gamma_vp_values, save_filename="gamma_vp_plot.png"):
    """Create and save the plot"""
    plt.figure(figsize=(10, 6))
    plt.plot(t_values, gamma_vp_values, 'b-', linewidth=2, label=r'$\gamma_{vp}$')
    plt.legend(loc='upper left', fontsize=12)
    plt.title(r'$\gamma_{vp}$ vs Time (t)', fontsize=14, fontweight='bold')
    plt.xlabel('Time (t)', fontsize=12)
    plt.ylabel(r'$\gamma_{vp}$', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.xticks(np.arange(0, 1.1, 0.1))
    
    # Dynamic y-axis scaling based on data
    y_max = max(gamma_vp_values)
    y_step = max(0.1, y_max / 20)  # Adaptive step size
    plt.yticks(np.arange(0, y_max + y_step, y_step))
    
    plt.tight_layout()
    plt.savefig(save_filename, dpi=300, bbox_inches='tight')
    print(f"Plot saved to {save_filename}")
    plt.show()

def main():
    """Main execution function"""
    # Time range from 0 to 1 with step 0.001
    t_values = np.arange(0.001, 1.001, 0.001)  # Start from 0.001 to avoid t=0 issues
    
    # Calculate gamma_vp for each time point (vectorized for better performance)
    gamma_vp_values = calculate_gamma_vp(t_values)
    
    # Save results to CSV
    save_to_csv(t_values, gamma_vp_values)
    
    # Create and save plot
    create_plot(t_values, gamma_vp_values)
    
    # Print summary statistics
    print(f"\nSummary Statistics:")
    print(f"- Number of data points: {len(t_values)}")
    print(f"- Time range: {t_values[0]:.3f} to {t_values[-1]:.3f}")
    print(f"- Gamma_vp range: {min(gamma_vp_values):.6f} to {max(gamma_vp_values):.6f}")
    print(f"- Mean gamma_vp: {np.mean(gamma_vp_values):.6f}")

if __name__ == "__main__":
    main()