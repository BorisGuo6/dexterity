import csv
import matplotlib.pyplot as plt

# Load the noisy data from the CSV file
with open("test/filtering_test/noisy_combined_sine_data.csv", "r") as file:
    reader = csv.reader(file)
    next(reader)  # Skip header
    data = [(float(row[0]), float(row[1])) for row in reader]

time = [row[0] for row in data]
noisy_data = [row[1] for row in data]

# Define the moving average filter
def moving_average_causal(data_point, buffer, window_size):
    """Applies a causal moving average filter to the current data point."""
    buffer.append(data_point)
    if len(buffer) > window_size:
        buffer.pop(0)
    return sum(buffer) / len(buffer)

# Real-time filtering setup
window_size = 20
buffer = []  # Circular buffer to store the last `window_size` data points
smoothed_data = []

# Apply the filter in a causal way
for point in noisy_data:
    smoothed_value = moving_average_causal(point, buffer, window_size)
    smoothed_data.append(smoothed_value)

# Plot the original noisy data and the smoothed data
plt.figure(figsize=(10, 4))
plt.plot(time, noisy_data, label="Noisy Data", alpha=0.5, color="blue")
plt.plot(time, smoothed_data, label="Smoothed Data (Moving Avg)", color="red", linewidth=2)
plt.title("Real-Time Moving Average Filter (Window Size = 5)")
plt.xlabel("Time (s)")
plt.ylabel("Data Value")
plt.legend()
plt.grid()
plt.show()

# Save the smoothed data to a new CSV file
with open("smoothed_data_causal.csv", "w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["Time", "Data"])
    writer.writerows(zip(time, smoothed_data))

print("Smoothed data saved to 'smoothed_data_causal.csv'.")