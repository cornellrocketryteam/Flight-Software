"""
apply_noise.py: Applies Gaussian noise to OpenRocket data for simulation purposes.
"""
import numpy as np
import pandas as pd
import sys

def apply_noise(input_csv, output_csv):
    alt_noise_std = 0.3
    acc_noise_std = 0.3

    data = pd.read_csv(input_csv)
    
    if "altitude" in data.columns and "acceleration_z" in data.columns:
        data["acceleration_z_noisy"] = (data["acceleration_z"] + np.random.normal(0, acc_noise_std, size=data.shape[0])).round(3)
        data["altitude_noisy"] = (data["altitude"] + np.random.normal(0, alt_noise_std, size=data.shape[0])).round(3)
        
        data[["timestamp", "altitude_noisy", "acceleration_z_noisy", ]].to_csv(output_csv, index=False)
    else:
        print("Error: Could not read CSV columns")


if __name__ == "__main__":
    input_csv = sys.argv[1]
    output_csv = input_csv[:input_csv.index(".")] + "_noisy.csv"

    apply_noise(input_csv, output_csv)
