import numpy as np
import matplotlib.pyplot as plt
from openpyxl import Workbook

def calculate_gamma_R_bar(q, A1, n1):
    """
    Calculate γ̄_R = (q/A1)^n1
    
    Parameters:
    -----------
    q : float or array
        Time-dependent parameter q
    A1 : float
        Model parameter A1
    n1 : float
        Exponent parameter n1
    
    Returns:
    --------
    gamma_R_bar : float or array
        Target value γ̄_R
    """
    return np.power(q / A1, n1)

def get_q_values(t, t_switch=15.0, q1=10.0, q2=5.0):
    """
    Get time-dependent q values.
    
    Parameters:
    -----------
    t : array-like
        Time points
    t_switch : float
        Time at which q switches from q1 to q2
    q1 : float
        q value for t <= t_switch
    q2 : float
        q value for t > t_switch
    
    Returns:
    --------
    q : array
        Time-dependent q values
    """
    t = np.asarray(t)
    q = np.where(t <= t_switch, q1, q2)
    return q

def analytical_solution(t, gamma_R0, A1, A2, n1, n2, A, B, n, m, t0=0, 
                               t_switch=15.0, q1=10.0, q2=5.0):
    """
    Compute the improved analytical solution γ_R(t) with dynamic γ̄_R.
    
    Parameters:
    -----------
    t : array-like
        Time points
    gamma_R0 : float
        Initial value γ_R0
    A1 : float
        Parameter A1 for γ̄_R calculation
    A2 : float
        Parameter A2 for Z calculation
    n1 : float
        Exponent n1 for γ̄_R calculation
    n2 : float
        Exponent n2 for Z calculation
    A, B : float
        Model parameters
    n, m : float
        Exponent parameters
    t0 : float, optional
        Initial time (default: 0)
    t_switch : float, optional
        Time at which q switches (default: 15.0)
    q1, q2 : float, optional
        q values before and after switch (default: 10.0, 5.0)
    
    Returns:
    --------
    gamma_R : array
        Solution γ_R(t) at time points t
    gamma_R_bar : array
        Time-dependent target values γ̄_R(t)
    q : array
        Time-dependent q values
    """
    
    t = np.asarray(t)
    
    # Get time-dependent q values
    q = get_q_values(t, t_switch, q1, q2)
    
    # Calculate time-dependent γ̄_R
    gamma_R_bar = calculate_gamma_R_bar(q, A1, n1)
    
    # Initialize solution array
    gamma_R = np.zeros_like(t)
    
    # Handle each time segment separately due to discontinuity at t_switch
    # Segment 1: t <= t_switch
    mask1 = t <= t_switch
    if np.any(mask1):
        t1 = t[mask1]
        gamma_R_bar1 = gamma_R_bar[mask1][0]  # Constant in this segment
        q1_val = q[mask1][0]  # q value for this segment
        gamma_R[mask1] = solve_segment(t1, gamma_R0, gamma_R_bar1, A, B, n, m, t0, 
                                     A1, A2, n1, n2, q1_val)
    
    # Segment 2: t > t_switch
    mask2 = t > t_switch
    if np.any(mask2):
        t2 = t[mask2]
        gamma_R_bar2 = gamma_R_bar[mask2][0]  # Constant in this segment
        q2_val = q[mask2][0]  # q value for this segment
        
        # Initial condition for second segment is the value at t_switch
        if np.any(mask1):
            gamma_R0_seg2 = gamma_R[mask1][-1]
        else:
            # If no previous segment, use original initial condition
            gamma_R0_seg2 = gamma_R0
            
        gamma_R[mask2] = solve_segment(t2, gamma_R0_seg2, gamma_R_bar2, A, B, n, m, t_switch, 
                                     A1, A2, n1, n2, q2_val)
    
    return gamma_R, gamma_R_bar, q

def solve_segment(t, gamma_R0, gamma_R_bar, A, B, n, m, t0, A1, A2, n1, n2, q_val):
    """
    Solve the analytical solution for a single segment with constant γ̄_R.
    
    Parameters:
    -----------
    t : array-like
        Time points for this segment
    gamma_R0 : float
        Initial value for this segment
    gamma_R_bar : float
        Constant target value for this segment
    A, B, n, m : float
        Model parameters
    t0 : float
        Initial time for this segment
    A1, A2 : float
        Parameters for Z calculation
    n1, n2 : float
        Exponents for Z calculation
    q_val : float
        Constant q value for this segment
    
    Returns:
    --------
    gamma_R : array
        Solution for this segment
    """
    
    t = np.asarray(t)
    
    # Determine regime based on γ_R0 vs γ̄_R
    if gamma_R0 <= gamma_R_bar:
        # Case: γ_R0 ≤ γ̄_R (growth)
        k = n
        U = A * np.power(1 - gamma_R0/gamma_R_bar, n-1)
    else:
        # Case: γ_R0 ≥ γ̄_R (decay)
        k = m
        U = B * np.power(gamma_R0/gamma_R_bar - 1, m-1)
    
    # Calculate Z - constant for this segment
    Z = (np.power(A1, n1) / np.power(A2, n2)) * np.power(q_val, (n2 - n1))
    
    # Calculate V(t - t0) - simplified form
    dt = np.maximum(t - t0, 0)
    V = (k - 1) * U * Z * dt
    
    # Handle case where k is close to 1
    if np.abs(k - 1) < 1e-10:
        # For k≈1, use exponential form
        xi = 1 - np.exp(-V)
    else:
        # Standard case: ξ(t) = 1 - (1/(1+V))^(1/(k-1))
        # Ensure V is positive to avoid numerical issues
        V = np.maximum(V, 0)
        xi = 1 - np.power(1 / (1 + V), 1/(k-1))
    
    # Calculate γ_R(t) using equation (17)
    gamma_R = gamma_R0 + (gamma_R_bar - gamma_R0) * xi
    
    return gamma_R

def save_results_to_excel(t, gamma_R, gamma_R_bar, q, filename="gamma_R_results.xlsx"):
    """
    Save the analysis results to an Excel file.
    
    Parameters:
    -----------
    t : array
        Time values
    gamma_R : array
        Gamma_R values
    gamma_R_bar : array
        Gamma_R_bar values (target values)
    q : array
        q parameter values
    filename : str
        Output Excel filename
    """
    # --- Save to Excel ---
    wb = Workbook()
    ws = wb.active
    ws.title = "Gamma_R Results"
    
    # Add headers
    ws.append(["Time (t)", "Gamma_R", "Gamma_R_bar", "q_parameter"])
    
    # Add data rows
    for t_val, gamma_r_val, gamma_r_bar_val, q_val in zip(t, gamma_R, gamma_R_bar, q):
        ws.append([float(t_val), float(gamma_r_val), float(gamma_r_bar_val), float(q_val)])
    
    # Save the workbook
    wb.save(filename)
    print(f"Results saved to {filename}")

def plot_improved_solution():
    """
    Plot the improved analytical solution with dynamic parameters.
    """
    # Time range from 0 to 30
    t = np.linspace(0, 30, 1000)
    
    # Parameters
    gamma_R0 = 0.0
    A1, A2 = 0.3, 0.5
    n1, n2 = 3.0, 4.0  
    A, B = 0.5, 0.05
    n, m = 5.0, 5.0
    
    # Calculate solution
    gamma_R, gamma_R_bar, q = analytical_solution(
        t, gamma_R0, A1, A2, n1, n2, A, B, n, m, 
        t_switch=15.0, q1=10.0, q2=5.0
    )
    
    # Create main plot only
    plt.figure(figsize=(10, 6))
    plt.plot(t, gamma_R, 'b-', linewidth=2, label='γ_R(t)')
    plt.plot(t, gamma_R_bar, 'r--', linewidth=2, label='γ̄_R(t) = (q/A₁)^n₁')
    plt.axhline(y=gamma_R0, color='g', linestyle=':', alpha=0.7, label='γ_R0 = 0')
    plt.axvline(x=15, color='k', linestyle='--', alpha=0.5, label='t = 15 (q switch)')
    plt.title('Analytical Solution: γ_R(t) and γ̄_R(t)', fontsize=14)
    plt.xlabel('Time t')
    plt.ylabel('γ_R, γ̄_R')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.show()
    
    # Save results to Excel
    save_results_to_excel(t, gamma_R, gamma_R_bar, q)
    
    return t, gamma_R, gamma_R_bar, q

def analyze_solution_properties(t, gamma_R, gamma_R_bar, q):
    """
    Analyze key properties of the solution.
    """
    print("Solution Analysis")
    print("="*50)
    print(f"Time range: {t[0]:.1f} to {t[-1]:.1f}")
    print(f"Initial γ_R: {gamma_R[0]:.4f}")
    print(f"Final γ_R: {gamma_R[-1]:.4f}")
    print(f"γ̄_R at t=0: {gamma_R_bar[0]:.4f}")
    print(f"γ̄_R at t=15⁻: {gamma_R_bar[t <= 15][-1]:.4f}")
    print(f"γ̄_R at t=15⁺: {gamma_R_bar[t > 15][0]:.4f}")
    print(f"γ̄_R at t=30: {gamma_R_bar[-1]:.4f}")
    
    # Find discontinuity magnitude
    idx_switch = np.where(t > 15)[0][0]
    gamma_R_jump = gamma_R_bar[idx_switch] - gamma_R_bar[idx_switch-1]
    print(f"γ̄_R discontinuity at t=15: {gamma_R_jump:.4f}")
    
    # Check if solution reaches equilibrium
    final_diff = abs(gamma_R[-1] - gamma_R_bar[-1])
    print(f"Final |γ_R - γ̄_R|: {final_diff:.4f}")
    
    print("\nq parameter switch:")
    print(f"q for t ≤ 15: {q[0]:.1f}")
    print(f"q for t > 15: {q[-1]:.1f}")

# Example usage and demonstration
if __name__ == "__main__":
    # Run the improved solution
    t, gamma_R, gamma_R_bar, q = plot_improved_solution()
    
    # Analyze the solution
    analyze_solution_properties(t, gamma_R, gamma_R_bar, q)