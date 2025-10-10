import pandas as pd
from src.ParameterToSurface import Surface
from src.ParameterToSurface import HeightMap
from tqdm import tqdm
from itertools import product
from matplotlib import pyplot as plt

import numpy as np
def variation_of_surface_parameters(runs : int):
    parameters = []
    for _ in tqdm(range(runs)):
        heightmap = HeightMap.HeightMap((200,200),(100.,100.))
        heightmap.random_complex(cluster_rounds=10, cluster_diameter=10., max_height=6., min_height=0.)
        surface = Surface.Surface(heightmap=heightmap)
        parameters.append([surface.mean_height, surface.root_mean_square_height, surface.kurtosis, surface.skewness])
    values = np.array(parameters)
    return np.column_stack((np.mean(values,axis=0), np.std(values,axis=0)))

def variation_of_parameters(**kwargs):
        arg_cluster_rounds = kwargs.get("cluster_rounds")
        arg_cluster_diameter = kwargs.get("cluster_diameter")
        arg_max_height = kwargs.get("max_height")
        arg_min_height = kwargs.get("min_height")
        arg_grid_size = kwargs.get("grid_size")
        arg_length = kwargs.get("length")
        arg_runs = kwargs.get("runs")
        combinations = list(product(arg_cluster_rounds, arg_cluster_diameter, arg_max_height, arg_min_height, arg_grid_size, arg_length))
        df = pd.DataFrame(columns=['cluster_rounds', 'cluster_diameter', 'max_height',
                                   'min_height', 'grid_size', 'length', 'runs',
                                   'Sv_mean', 'Sv_std', 'Sq_mean', 'Sq_std',
                                   'Sku_mean', 'Sku_std', 'Ssk_mean', 'Ssk_std'])
        for cluster_rounds, cluster_diameter, max_height, min_height, grid_size, length in tqdm(combinations,
                                                                                                desc='parameters',
                                                                                                position=0,
                                                                                                disable=False,
                                                                                                ascii=True,
                                                                                                ncols=80):
            parameters = []
            for _ in tqdm(range(arg_runs),desc='runs', leave=False, position=1, ascii=True, ncols=80):
                heightmap = HeightMap.HeightMap(grid_size,length)
                heightmap.random_complex(cluster_rounds=cluster_rounds, cluster_diameter=cluster_diameter,
                                         max_height=max_height, min_height=min_height)
                surface = Surface.Surface(heightmap=heightmap)
                parameters.append([surface.mean_height, surface.root_mean_square_height, surface.kurtosis, surface.skewness])
            values = np.array(parameters)
            mean = np.mean(values,axis=0)
            std = np.std(values,axis=0)
            df.loc[len(df)] = [cluster_rounds, cluster_diameter, max_height, min_height, grid_size, length, arg_runs,
                               float(mean[0]), float(std[0]), float(mean[1]), float(std[1]),
                               float(mean[2]), float(std[2]), float(mean[3]), float(std[3])]

        return df

def plot_data(df, parameter, save : str | None = None):
    possible_parameters = ['cluster_rounds', 'cluster_diameter', 'max_height', 'min_height', 'grid_size', 'length']
    if parameter not in possible_parameters:
        raise ValueError(f'Parameter {parameter} not recognized.')
    plot_parameters = possible_parameters
    plot_parameters.remove(parameter)
    df_plot_parameters = df[plot_parameters]
    df_plot_parameters = df_plot_parameters.drop_duplicates()
    if df_plot_parameters.shape[0] != 1:
        raise ValueError(f'Dataframe has multiple different values for rest of parameters')

    fig, axes = plt.subplots(2,2, figsize=(10,10), sharex=True)
    axes = axes.flatten()
    metrics = ['Sv','Sq','Sku','Ssk']
    for ax, metric in zip(axes, metrics):
        mean_col = f"{metric}_mean"
        std_col = f"{metric}_std"

        ax.errorbar(
            df[parameter],
            df[mean_col],
            yerr=df[std_col],
            fmt='o-',
            capsize=5,
            ecolor='gray',
            elinewidth=1,
            label=f"{metric}_mean ± {metric}_std"
        )

        ax.set_title(metric)
        ax.set_xlabel("Cluster Rounds")
        ax.set_ylabel(metric)
        ax.grid(True)
        ax.legend()


    print(df_plot_parameters)
    extra_text = df_plot_parameters.T.to_string(index=True)
    fig.text(0.5, 0.01, extra_text, ha='center', fontsize=12)
    plt.suptitle(f"Surface Parameters vs {parameter}", fontsize=14)
    plt.tight_layout(rect=(0., 0.1, 1., 0.95))
    if save is not None:
        plt.savefig(save, dpi=300)
    else:
        plt.show()

def variation_cluster_rounds():
    print("Test cluster_rounds")
    df = variation_of_parameters(
        cluster_rounds=[10,20,50,100,200,500,1000],
        cluster_diameter=[10.],
        max_height=[6.],
        min_height=[0.],
        grid_size=[(200,200)],
        length=[(100.,100.)],
        runs=10)
    plot_data(df, "cluster_rounds", "cluster_rounds")

def variation_cluster_diameter():
    print("Test cluster_diameter")
    df = variation_of_parameters(
        cluster_rounds=[500],
        cluster_diameter=[1.,2.,5.,10.,20.],
        max_height=[6.],
        min_height=[0.],
        grid_size=[(200,200)],
        length=[(100.,100.)],
        runs=10)
    plot_data(df, "cluster_diameter", "cluster_diameter")

def variation_max_height():
    print("Test max_height")
    df = variation_of_parameters(
        cluster_rounds=[500],
        cluster_diameter=[10.],
        max_height=[0.1,0.2,0.5,1.,2.,5.,10.,20.,50.],
        min_height=[0.],
        grid_size=[(200,200)],
        length=[(100.,100.)],
        runs=10)
    plot_data(df, "max_height", "max_height")

if __name__ == '__main__':
    variation_cluster_rounds()
    #variation_cluster_diameter()
    #variation_max_height()
