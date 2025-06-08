# computes the Q and P stress values from the X = Q - P (MPa) and Y = (Q + 2P)/3(MPa) 
# stress values shown on the graph in Fig. 7 and Fig. 8 of Azabou et al. (2021)
import csv

# Example data for multiple rows: (time, X, Y)
data = [
    (10, 4.0, 10.0),
    (40, 7.5, 10.0),
    (90, 15, 10.0),
    (140, 18, 10.0),
    (170, 21, 10.0),
    # Add more rows as needed
]

# Open a CSV file for writing
with open('stress_values.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    
    # Write the header row
    writer.writerow(["Time (%)", "X = Q - P (MPa)", "Y = (Q + 2P)/3 (MPa)", "Q = X + P (MPa)", "P = (3Y - X)/3 (MPa)"])
    
    # Calculate P and Q for each row and write to the CSV file
    for time, X, Y in data:
        P = (3 * Y - X) / 3
        Q = X + P
        writer.writerow([time, X, Y, Q, P])  # Include time in the output

print("Data has been written to 'stress_values.csv'")