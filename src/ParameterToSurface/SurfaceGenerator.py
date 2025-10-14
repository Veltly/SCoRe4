#!/usr/bin/env python3
import argparse
import yaml
from pathlib import Path
import sys

from src.ParameterToSurface.GDMLWriter import GDMLWriter
from src.ParameterToSurface.HeightMap import HeightMap, HeightMapParameters
from src.ParameterToSurface.Surface import Surface

# --- Config values with type and default argument ---
#define all config values here. Is later used by argparse and class Config.
surface_params = {
    "grid_nx": [int, 10],
    "grid_ny": [int, 10],
    "grid_dx": [float, 1.],
    "grid_dy": [float, 1.],
    "point_cluster_rounds": [int, 0],
    "point_cluster_diameter": [float, 0.1],
    "length_cluster_rounds": [int, 0],
    "length_cluster_length": [float, 0.3],
    "length_cluster_width": [float, 0.1],
    "even_out_rounds": [int, 0],
    "edge_height": [float, 5.],
    "max_height": [float, 10.],
    "min_height": [float, 0.],
    "seed": [int, None],
    "export_name": [str, "default"],
    "solid_name": [str, "solid"]
}

class Config:
    DEFAULT_CONFIG_VALUES = {key: value[1] for key, value in surface_params.items()}

    DEFAULT_VALUES = {
        "default_config_path": "./default_config.yaml"
    }

    def __init__(self):
        self.config = Config.DEFAULT_CONFIG_VALUES
        self.load_default_config_file()

    def update_config(self, new_config: dict) -> None:
        Config._update(self.config, new_config)

    @staticmethod
    def _update(base: dict, user_config: dict) -> dict:
        for key, value in user_config.items():
            if isinstance(value, dict) and key in base and isinstance(base[key], dict):
                Config._update(base[key], value)
            else:
                base[key] = value
        return base

    @staticmethod
    def _load_config_file_method(path: str) -> dict | None:
        path = Path(path)
        if path.exists():
            with open(path, "r", encoding="utf-8") as f:
                data = yaml.safe_load(f) or {}
            return data
        else:
            print(f"No default config file '{path}' found!")
        return None

    def load_config_file(self, path:str) -> None:
        config = Config._load_config_file_method(path)
        if config is None:
            print(f"No config file found at {path}")
            return
        self.update_config(config)

    def load_default_config_file(self) -> None:
        default_config_path = Config.DEFAULT_VALUES["default_config_path"]
        self.load_config_file(default_config_path)

    @staticmethod
    def _write_config_to_file(config : dict, path: str) -> None:
        target = Path(path)
        if not target:
            raise ValueError("Path not valid")
        target.parent.mkdir(parents=True, exist_ok=True)
        with open(target, "w", encoding="utf-8") as f:
            yaml.safe_dump(config, f, sort_keys=False, indent=2)
        print(f"Default config written to {target}")

    def write_config_file(self, path : str) -> None:
        Config._write_config_to_file(self.config, path)

    def write_to_default_config(self) -> None:
        path = Config.DEFAULT_VALUES["default_config_path"]
        Config._write_config_to_file(self.config, path)

    def setup_default_config_file(self):
        path = Config.DEFAULT_VALUES["default_config_path"]
        config = Config.DEFAULT_CONFIG_VALUES
        self._write_config_to_file(config, path)

    def __getitem__(self,key):
        return self.config[key]

    def __setitem__(self,key,value):
        self.config[key] = value


# control flow of program after configuration is generated
def exec_program(config: Config, control) -> None:
    if control["config_only"]:
        return

    heightmap = None
    if control["heightmap"] or control["parameters"] or control["surface"] or control["gdml"]:
        # --- Generate height map
        heightmap = HeightMap(n=(config["grid_nx"], config["grid_ny"]),length=(config["length_dx"], config["length_dy"]))

        heightmap_params = HeightMapParameters()
        heightmap_params.point_cluster_rounds = config["point_cluster_rounds"]
        heightmap_params.point_cluster_diameter = config["point_cluster_diameter"]
        heightmap_params.length_cluster_rounds = config["length_cluster_rounds"]
        heightmap_params.length_cluster_length = config["length_cluster_length"]
        heightmap_params.length_cluster_width = config["length_cluster_width"]
        heightmap_params.even_out_rounds = config["even_out_rounds"]
        heightmap_params.edge_height = config["edge_height"]
        heightmap_params.max_height = config["max_height"]
        heightmap_params.min_height = config["min_height"]
        heightmap_params.seed = config["seed"]

        heightmap.random_complex_all(heightmap_params)

    if control["heightmap"]:
        plot_heightmap_export_path = str(config["export_path"]) + "_heightmap"
        show_plot = ~control["silent"]
        heightmap.plot(plot_heightmap_export_path, show=show_plot)

    surface = None
    if control["surface"] or control["parameters"] or control["gdml"]:
        surface = Surface(heightmap=heightmap)

        if control["surface"] and not control["silent"]:
            surface.show()

        if control["parameters"]:
            print(surface)

    if control["gdml"]:
        gdml_filename = config["export_name"]
        solid_name = config["solid_name"]
        GDMLWriter(surface.mesh.vertices, surface.mesh.faces, solid_name, gdml_filename)


def main():
    parser = argparse.ArgumentParser(description='CLI tool to generate surface',
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    # --- Configuration control ---
    config_control = parser.add_argument_group("Configuration control")
    config_control.add_argument("--setup", help="Creates default config file", action="store_true")
    config_control.add_argument("--config", help="Path to configuration file", type=str)
    config_control.add_argument("--write", help="Write configuration to file", type=str)
    config_control.add_argument("--write-default", help="Write config to default config file", action="store_true")

    # --- Program control flow ---
    control_group = parser.add_argument_group("Program control")
    control_params = {
        "config_only": "Updates only configuration",
        "heightmap": "Plot height map",
        "parameters": "Calculate parameters",
        "surface": "Plot surface",
        "gdml": "Generate GDML file",
        "silent": "Generate no graphical output"
    }
    for name, description in control_params.items():
        control_group.add_argument(f"--{name.replace('_','-')}", help=f"{description}", action="store_true", default=False)

    # --- Surface control options ---
    surface_group = parser.add_argument_group("Surface control options",
                                                description="Use of arguments override config file values!")
    for name, value in surface_params.items():
        typ = value[0]
        surface_group.add_argument(f"--{name.replace('_','-')}", help=f"{name.replace('_',' ')}", type=typ)

    # --- Parse args ---
    if len(sys.argv) == 1:
        parser.print_help(sys.stderr)
        sys.exit(1)

    args = parser.parse_args()
    config = Config()

    # --- Setup default config ---
    if args.setup:
        config.setup_default_config_file()
        print("Setup default config file")

    # --- Load custom config file ---
    if args.config:
        config.load_config_file(args.config)
        print(f"Loading config file from {args.config}")

    # --- Override config with CLI arguments ---
    surface_overrides = { key: getattr(args, key) for key in surface_params if getattr(args, key) is not None }
    config.update_config(surface_overrides)

    # --- Write config to custom config file ---
    if args.write:
        config.write_config_file(args.write)
        print(f"Writing config to {args.write}")

    # --- Write config to default config file ---
    if args.write_default:
        config.write_to_default_config()
        print("Writing config to default config file")

    # --- Execute program ---
    control = {key: getattr(args, key) for key in control_params if getattr(args, key)}
    exec_program(config, control)

if __name__ == "__main__":
    main()