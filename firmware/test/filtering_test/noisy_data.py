import numpy as np
import matplotlib.pyplot as plt

# Parameters
sampling_rate = 50  # Hz
duration = 10  # seconds
data_range = (0, 255)
noise_range = (-10, 10)

# Generate time values
time = np.linspace(0, duration, int(sampling_rate * duration), endpoint=False)

# Define two sine waves with different frequencies and amplitudes
frequency1 = .13  # Hz
frequency2 = .6  # Hz
amplitude1 = 75  # Amplitude for sine wave 1
amplitude2 = 35   # Amplitude for sine wave 2

# Generate the two sine waves
sine_wave1 = amplitude1 * np.sin(2 * np.pi * frequency1 * time)+ amplitude1
sine_wave2 = amplitude2 * np.sin(2 * np.pi * frequency2 * time) + amplitude2

# Combine the sine waves
combined_wave = sine_wave1 + sine_wave2

# Add random noise in the range -30 to 30
noise = np.random.randint(noise_range[0], noise_range[1] + 1, size=len(time))

# Add noise to the combined sine wave and clip to keep within the range of 0-255
noisy_data = np.clip(combined_wave + noise, data_range[0], data_range[1])

# Plot the noisy data
plt.figure(figsize=(10, 4))
plt.plot(time, noisy_data, label="Noisy Combined Sine Waves", color="blue", alpha=0.7)
plt.title("Combined Sine Waves with Noise (0-255)")
plt.xlabel("Time (s)")
plt.ylabel("Data Value")
plt.grid()
plt.legend()
plt.show()

# Save data to a CSV file
np.savetxt("test/filtering_test/noisy_combined_sine_data.csv", np.column_stack((time, noisy_data)), delimiter=",", header="Time,Data", comments="")

print("Noisy combined sine wave data generated and saved to 'noisy_combined_sine_data.csv'.")