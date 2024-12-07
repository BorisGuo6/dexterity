import numpy as np

def calibrate_hall_effect_sensor(sensor_values):
    """
    Calibrates a Hall-effect sensor using a second-degree polynomial fit.
    
    Args:
        sensor_values (list of float): The sensor's raw output values corresponding to angles
                                       [0°, 45°, 90°, 135°, 180°].

    Returns:
        str: A formatted string of polynomial coefficients in the format {a, b, c}.
    """
    if len(sensor_values) != 5:
        raise ValueError("Please provide exactly 5 sensor values corresponding to [0°, 45°, 90°, 135°, 180°].")
    
    # Define the angles corresponding to the sensor values
    angles = [0, 45, 90, 135, 180]
    
    # Perform a second-degree polynomial fit
    coefficients = np.polyfit(sensor_values, angles, 2)  # Fit angles as a function of sensor values
    
    # Reverse coefficients for proper format (a*x^2 + b*x + c)
    formatted_coefficients = f"{{{coefficients[0]:.15f},{coefficients[1]:.15f},{coefficients[2]:.15f}}}"
    
    return formatted_coefficients

# Example usage:
example_values = [2040, 2110, 2220, 2370, 2500]  # Replace with your actual sensor readings
polynomial = calibrate_hall_effect_sensor(example_values)
print(f"Polynomial fit: {polynomial}")
