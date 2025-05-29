import numpy as np
import matplotlib.pyplot as plt

def calculate_gamma_R_bar(q, A1, n1):
    return (q / A1) ** n1

def get_q_values(t, t_switch=15.0, q1=10.0, q2=5.0):
    if t <= t_switch:
        return q1
    else:
        return q2

def solve_segment(t, gamma_R0, gamma_R_bar, A, B, n, m, t0, A1, A2, n1, n2, q_val):
    # Determine regime
    if gamma_R0 <= gamma_R_bar:
        k = n
        U = A * ((1 - gamma_R0/gamma_R_bar) ** (n-1))
    else:
        k = m
        U = B * ((gamma_R0/gamma_R_bar - 1) ** (m-1))
    
    # Calculate Z
    Z = (A1 ** n1) / (A2 ** n2) * (q_val ** (n2 - n1))
    
    # Calculate V
    dt = max(t - t0, 0)
    V = (k - 1) * U * Z * dt
    
    # Calculate xi
    if abs(k - 1) < 1e-10:
        xi = 1 - np.exp(-V)
    else:
        V = max(V, 0)
        xi = 1 - (1 / (1 + V)) ** (1/(k-1))
    
    # Calculate gamma_R
    gamma_R = gamma_R0 + (gamma_R_bar - gamma_R0) * xi
    return gamma_R

def analytical_solution(t_array, gamma_R0, A1, A2, n1, n2, A, B, n, m, t_switch=15.0, q1=10.0, q2=5.0):
    gamma_R_results = []
    gamma_R_bar_results = []
    q_results = []
    
    # Process first segment (t <= t_switch)
    gamma_R_at_switch = gamma_R0
    
    for i, t in enumerate(t_array):
        q = get_q_values(t, t_switch, q1, q2)
        gamma_R_bar = calculate_gamma_R_bar(q, A1, n1)
        
        if t <= t_switch:
            # First segment
            gamma_R = solve_segment(t, gamma_R0, gamma_R_bar, A, B, n, m, 0, A1, A2, n1, n2, q1)
            if i == len([x for x in t_array if x <= t_switch]) - 1:
                gamma_R_at_switch = gamma_R
        else:
            # Second segment
            gamma_R = solve_segment(t, gamma_R_at_switch, gamma_R_bar, A, B, n, m, t_switch, A1, A2, n1, n2, q2)
        
        gamma_R_results.append(gamma_R)
        gamma_R_bar_results.append(gamma_R_bar)
        q_results.append(q)
    
    return gamma_R_results, gamma_R_bar_results, q_results

def save_results_to_csv(t, gamma_R, gamma_R_bar, q, filename="MunsonDawson_analytical.csv"):
    with open(filename, 'w') as f:
        f.write("Time (t),Gamma_R,Gamma_R_bar,q_parameter\n")
        for i in range(len(t)):
            f.write(f"{t[i]},{gamma_R[i]},{gamma_R_bar[i]},{q[i]}\n")
    print(f"Results saved to {filename}")

def plot_solution():
    # Time range
    t = np.linspace(0, 30, 1000)
    
    # Parameters
    gamma_R0 = 0.0
    A1, A2 = 0.3, 0.5
    n1, n2 = 3.0, 4.0  
    A, B = 0.5, 0.05
    n, m = 5.0, 5.0
    
    # Calculate solution
    gamma_R, gamma_R_bar, q = analytical_solution(t, gamma_R0, A1, A2, n1, n2, A, B, n, m)
    
    # Plot
    plt.figure(figsize=(10, 6))
    plt.plot(t, gamma_R, 'b-', linewidth=2, label='γ_R(t)')
    plt.plot(t, gamma_R_bar, 'r--', linewidth=2, label='γ̄_R(t)')
    plt.axhline(y=gamma_R0, color='g', linestyle=':', label='γ_R0 = 0')
    plt.axvline(x=15, color='k', linestyle='--', label='t = 15 (q switch)')
    plt.title('Analytical Solution')
    plt.xlabel('Time t')
    plt.ylabel('γ_R, γ̄_R')
    plt.legend()
    plt.grid(True)
    plt.show()
    
    # Save results
    save_results_to_csv(t, gamma_R, gamma_R_bar, q)
    
    return t, gamma_R, gamma_R_bar, q

# def analyze_solution(t, gamma_R, gamma_R_bar, q):
#     print("Solution Analysis")
#     print("="*50)
#     print(f"Time range: {t[0]:.1f} to {t[-1]:.1f}")
#     print(f"Initial γ_R: {gamma_R[0]:.4f}")
#     print(f"Final γ_R: {gamma_R[-1]:.4f}")
#     print(f"q for t ≤ 15: {q[0]:.1f}")
#     print(f"q for t > 15: {q[-1]:.1f}")

# Run the solution
if __name__ == "__main__":
    t, gamma_R, gamma_R_bar, q = plot_solution()
    # analyze_solution(t, gamma_R, gamma_R_bar, q)