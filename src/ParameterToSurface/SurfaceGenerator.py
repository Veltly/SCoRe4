#!/usr/bin/env python3
import argparse
import yaml
from pathlib import Path
import sys

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
    "export_path": [str, "surface_default"],
    "plot": [bool, False],
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

    #generate height map
    #plot height map + stop
    #generate surface
    #plot surface + stop
    #plot info + stop
    #generate gdml
    pass


def main():
    parser = argparse.ArgumentParser(description='CLI tool to generate surface',
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    # --- Configuration control ---
    config_control = parser.add_argument_group("Configuration control")
    config_control.add_argument("--setup", help="Creates default config file", action="store_true")
    config_control.add_argument("--config", help="Path to configuration file", type=str)
    config_control.add_argument("--write", help="Write configuration to file", type=str)
    config_control.add_argument("--write-default", help="Write config to default config file", action="store_true")

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
    #exec_program(config, control)

if __name__ == "__main__":
    main()