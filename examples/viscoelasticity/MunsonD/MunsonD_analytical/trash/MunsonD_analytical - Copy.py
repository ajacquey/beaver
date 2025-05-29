import numpy as np
import matplotlib.pyplot as plt

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

def analytical_solution_improved(t, gamma_R0, A1, n1, A, B, n, m, t0=0, 
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
    n1 : float
        Exponent n1 for γ̄_R calculation
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
        gamma_R[mask1] = solve_segment(t1, gamma_R0, gamma_R_bar1, A, B, n, m, t0)
    
    # Segment 2: t > t_switch
    mask2 = t > t_switch
    if np.any(mask2):
        t2 = t[mask2]
        gamma_R_bar2 = gamma_R_bar[mask2][0]  # Constant in this segment
        
        # Initial condition for second segment is the value at t_switch
        if np.any(mask1):
            gamma_R0_seg2 = gamma_R[mask1][-1]
        else:
            # If no previous segment, use original initial condition
            gamma_R0_seg2 = gamma_R0
            
        gamma_R[mask2] = solve_segment(t2, gamma_R0_seg2, gamma_R_bar2, A, B, n, m, t_switch)
    
    return gamma_R, gamma_R_bar, q

def solve_segment(t, gamma_R0, gamma_R_bar, A, B, n, m, t0):
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
    
    # Calculate V(t - t0) - simplified form
    # Assuming V = (k-1)U * (t-t0) for this implementation
    dt = np.maximum(t - t0, 0)
    V = (k - 1) * U * dt
    
    # Handle case where k is close to 1
    if np.abs(k - 1) < 1e-10:
        # For k≈1, use exponential form
        xi = 1 - np.exp(-V)
    else:
        # Standard case: ξ(t) = 1 - (1/(1+V))^(1/(k-1))
        xi = 1 - np.power(1 / (1 + V), 1/(k-1))
    
    # Calculate γ_R(t) using equation (17)
    gamma_R = gamma_R0 + (gamma_R_bar - gamma_R0) * xi
    
    return gamma_R

def plot_improved_solution():
    """
    Plot the improved analytical solution with dynamic parameters.
    """
    # Time range from 0 to 30
    t = np.linspace(0, 30, 1000)
    
    # Parameters
    gamma_R0 = 0.1
    A1 = 2.0
    n1 = 1.5
    A, B = 1.0, 1.0
    n, m = 2.0, 2.0
    
    # Calculate solution
    gamma_R, gamma_R_bar, q = analytical_solution_improved(
        t, gamma_R0, A1, n1, A, B, n, m, 
        t_switch=15.0, q1=10.0, q2=5.0
    )
    
    # Create comprehensive plot
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    fig.suptitle('Improved Analytical Solution with Dynamic Parameters', fontsize=16)
    
    # Main solution plot
    ax = axes[0, 0]
    ax.plot(t, gamma_R, 'b-', linewidth=2, label='γ_R(t)')
    ax.plot(t, gamma_R_bar, 'r--', linewidth=2, label='γ̄_R(t) = (q/A₁)^n₁')
    ax.axhline(y=gamma_R0, color='g', linestyle=':', alpha=0.7, label='γ_R0')
    ax.axvline(x=15, color='k', linestyle='--', alpha=0.5, label='t = 15 (q switch)')
    ax.set_title('Main Solution: γ_R(t) and γ̄_R(t)')
    ax.set_xlabel('Time t')
    ax.set_ylabel('γ_R, γ̄_R')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    # q parameter evolution
    ax = axes[0, 1]
    ax.plot(t, q, 'g-', linewidth=2, label='q(t)')
    ax.axvline(x=15, color='k', linestyle='--', alpha=0.5, label='t = 15')
    ax.set_title('Parameter q(t) Evolution')
    ax.set_xlabel('Time t')
    ax.set_ylabel('q')
    ax.legend()
    ax.grid(True, alpha=0.3)
    ax.set_ylim([4, 11])
    
    # Phase portrait style plot
    ax = axes[1, 0]
    ax.plot(gamma_R_bar, gamma_R, 'purple', linewidth=2, label='Solution trajectory')
    ax.plot(gamma_R_bar[0], gamma_R[0], 'go', markersize=8, label='Start')
    ax.plot(gamma_R_bar[-1], gamma_R[-1], 'ro', markersize=8, label='End')
    # Add diagonal line y=x
    min_val = min(np.min(gamma_R_bar), np.min(gamma_R))
    max_val = max(np.max(gamma_R_bar), np.max(gamma_R))
    ax.plot([min_val, max_val], [min_val, max_val], 'k--', alpha=0.5, label='γ_R = γ̄_R')
    ax.set_title('Phase Portrait: γ_R vs γ̄_R')
    ax.set_xlabel('γ̄_R(t)')
    ax.set_ylabel('γ_R(t)')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    # Parameter sensitivity analysis
    ax = axes[1, 1]
    for A1_val in [1.5, 2.0, 2.5]:
        gamma_R_sens, _, _ = analytical_solution_improved(
            t, gamma_R0, A1_val, n1, A, B, n, m
        )
        ax.plot(t, gamma_R_sens, linewidth=2, label=f'A₁ = {A1_val}')
    ax.axvline(x=15, color='k', linestyle='--', alpha=0.5, label='t = 15')
    ax.set_title('Sensitivity to Parameter A₁')
    ax.set_xlabel('Time t')
    ax.set_ylabel('γ_R(t)')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.show()
    
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