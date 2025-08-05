#./G4Surface/venv/bin/python

## plot generated 1D histogram from surfaceShift simulation

import pandas as pd
from matplotlib import pyplot as plt
def main():
    df = pd.read_csv("./depth_h1_depth.csv", header=6)
    data = df['entries'].values
    x = [i for i in range(0, len(df))]
    plt.step(x, data)
    plt.show()

if __name__ == "__main__":
    main()
