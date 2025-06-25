def calculate_epsilon(γ, ζ):
    """
    Solves for ε_ax and ε_r given γ and ζ.
    
    Parameters:
    γ (float): The value of γ.
    ζ (float): The value of ζ.
    
    Returns:
    tuple: (ε_ax, ε_r)
    """
    # Solving the system of equations:
    # γ = (2/3)(ε_ax - ε_r)
    # ζ = 2ε_r + ε_ax
    
    # From the first equation: ε_ax = (3/2)γ + ε_r
    # Substitute ε_ax into the second equation:
    # ζ = 2ε_r + (3/2)γ + ε_r
    # ζ = 3ε_r + (3/2)γ
    # ε_r = (ζ - (3/2)γ) / 3
    
    ε_r = (ζ - (3/2) * γ) / 3
    ε_ax = (3/2) * γ + ε_r
    
    return ε_ax, ε_r

def compute_t(ε_ax, ε_ax_dot):
    """
    Computes t = |ε_ax / ε_ax_dot|
    
    Parameters:
    ε_ax (float): The value of ε_ax.
    ε_ax_dot (float): The value of ε_ax_dot.
    
    Returns:
    float: The value of t.
    """
    t = abs(ε_ax / ε_ax_dot)
    return t

def seconds_to_days(t_seconds):
    """
    Converts time from seconds to days.
    
    Parameters:
    t_seconds (float): Time in seconds.
    
    Returns:
    float: Time in days.
    """
    return t_seconds / 86400  # 86400 seconds in a day

# Example usage:
γ = 0.08  # Example value for γ
ζ = -0.01  # Example value for ζ
ε_ax_dot = 1e-6  # Example value for ε_ax_dot

# Calculate ε_ax and ε_r
ε_ax, ε_r = calculate_epsilon(γ, ζ)

# Compute t in seconds
t_seconds = compute_t(ε_ax, ε_ax_dot)

# Convert t to days
t_days = seconds_to_days(t_seconds)

# Print the results
print(f"ε_ax: {ε_ax}")
print(f"ε_r: {ε_r}")
print(f"t (seconds): {t_seconds}")
print(f"t (days): {t_days}")