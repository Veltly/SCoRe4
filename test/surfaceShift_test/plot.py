#! /bin/python3


def main():
    import pandas as pd
    from matplotlib import pyplot as plt
    df = pd.read_csv("depth_h1_depth.csv", header=6)
    data = df['entries'].values
    x = [i for i in range(0, len(df))]
    plt.plot(x, data, marker='.')
    plt.show()


if __name__ == "__main__":
    main()
