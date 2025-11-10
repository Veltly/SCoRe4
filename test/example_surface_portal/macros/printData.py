#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def plot_energy_histogram():
    filename_shell = "DepositedEnergyOutput_h1_Shell.csv"
    filename_cube = "DepositedEnergyOutput_h1_Cube.csv"
    min_energy = 0
    max_energy = 10
    # Read CSV
    data_shell = pd.read_csv(filename_shell,skiprows=6)
    data_cube = pd.read_csv(filename_cube, skiprows=6)
    counts_shell = data_shell.entries
    counts_cube = data_cube.entries
    if(len(counts_shell) != len(counts_cube)):
        raise Exception("Data has not same length!")
    n_bins = len(counts_shell)
    energies = np.linspace(min_energy, max_energy, n_bins+1)

    # Plot histogram
    plt.stairs(counts_shell, energies, edgecolor='red', label='shell')
    plt.stairs(counts_cube, energies, edgecolor='blue', label='cube')
    plt.title(f"Energy Histogram ({min_energy} to {max_energy} MeV)")
    plt.xlabel("Energy [MeV]")
    plt.ylabel("Counts")
    plt.yscale("log")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.show()

if __name__ == "__main__":
    plot_energy_histogram()