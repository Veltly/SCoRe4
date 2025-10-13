#!/usr/bin/env python3
import argparse
import yaml
from pathlib import Path
from HeightMap import HeightMapParameters

class Config:
    DEFAULT_CONFIG_VALUES = {
        "grid_nx": 10,
        "grid_ny": 10,
        "grid_dx": 1.,
        "grid_dy": 1.,
        "point_cluster_rounds": 0,
        "point_cluster_diameter": 0.2,
        "length_cluster_rounds": 0,
        "length_cluster_length": 0.3,
        "length_cluster_width": 0.1,
        "even_out_rounds": 5,
        "edge_height": 5.,
        "max_height": 10.,
        "min_height": 0.,
        "seed": None,
        "export_path": "surface_default",
        "plot": True,
    }

    DEFAULT_VALUES = {
        "default_config_path": "./"
    }

    def __init__(self, config_path: str | None = None, args: argparse.Namespace | None = None):
        self.config = self.load_default_config_file()
        if config_path is not None:
            user_config = self.load_config_file(config_path)
            self.config = Config.update(self.config, user_config)

    @staticmethod
    def update(base: dict, user_config: dict) -> dict:
        for key, value in user_config.items():
            if isinstance(value, dict) and key in base and isinstance(base[key], dict):
                Config.update(base[key], value)
            else:
                base[key] = value
        return base

    @staticmethod
    def load_config_file(path: str) -> dict:
        path = Path(path)
        if not path.exists():
            raise FileNotFoundError(path)
        with open(path, "r", encoding="utf-8") as f:
            data = yaml.safe_load(f) or {}
        return data

    def load_default_config_file(self) -> dict:
        default_config = self.load_config_file(Config.DEFAULT_VALUES["default_config_path"])
        return default_config

    def setup_default_config_file(self) -> dict:
        pass

def main():
    parser = argparse.ArgumentParser(description='CLI tool to generate surface')
    parser.add_argument("config", help="Path to configuration file")
    args = parser.parse_args()

if __name__ == "__main__":
    main()