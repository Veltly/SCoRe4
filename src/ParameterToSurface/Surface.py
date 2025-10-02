from dataclasses import dataclass
from enum import Enum
from abc import ABC, abstractmethod
from tqdm import tqdm
from line_profiler import LineProfiler

import numpy as np
import trimesh

@dataclass
class Coordinate:
    x: float
    y: float
    z: float
    def __add__(self, other):
        if isinstance(other, Coordinate):  # vector + vector
            return Coordinate(self.x + other.x, self.y + other.y, self.z + other.z)
        elif isinstance(other, (int, float)):  # vector + scalar
            return Coordinate(self.x + other, self.y + other, self.z + other)
        return NotImplemented

    __radd__ = __add__
    def __sub__(self, other):
        if isinstance(other, Coordinate):  # vector + vector
            return Coordinate(self.x - other.x, self.y - other.y, self.z - other.z)
        elif isinstance(other, (int, float)):  # vector + scalar
            return Coordinate(self.x - other, self.y - other, self.z - other)
        return NotImplemented

    __rsub__ = __sub__

    def __mul__(self, other):
        return Coordinate(self.x * other, self.y * other, self.z * other)
    def __repr__(self):
        return f"Coordinate({self.x}, {self.y}, {self.z})"
    def __str__(self):
        return f"({self.x}, {self.y}, {self.z})"

    def __array__(self):
        return np.array([self.x, self.y, self.z])
@dataclass
class SurfaceParameters:
    """Class to store all surface parameters"""
    mean_height: float
    root_mean_square_height: float
    skewness: float
    kurtosis: float


class SpikeForm(Enum):
    """Enum to represent spike forms"""
    STANDARD = 0
    PEAK = 1
    BUMP = 2


@dataclass
class SurfaceDescriptionOptions:
    """Class to store surface description"""
    spike_width: float = 1.0
    spike_mean_height: float = 1.0
    spike_number: int = 1
    spike_height_deviation: float = 0
    spike_layers: int = 1
    spike_form: SpikeForm = SpikeForm.STANDARD

class Volume(ABC):
    """Class to represent volume"""
    @abstractmethod
    def vertices(self) -> np.ndarray:
        pass
    @abstractmethod
    def faces(self) -> np.ndarray:
        pass
    @abstractmethod
    def type(self) -> SpikeForm:
        pass
    @staticmethod
    @abstractmethod
    def number_of_vertices() -> int:
        pass
    @staticmethod
    @abstractmethod
    def number_of_faces() -> int:
        pass


class PyramidVolume(Volume):
    def __init__(self, height: float, width: float, position: Coordinate):
        self.position = position
        self.height = height
        self.width = width
    def vertices(self) -> np.ndarray:
        vertex_0 = self.position + Coordinate(-self.width, -self.width,0) * 0.5
        vertex_1 = self.position + Coordinate(self.width, -self.width,0) * 0.5
        vertex_2 = self.position + Coordinate(self.width, self.width,0) * 0.5
        vertex_3 = self.position + Coordinate(-self.width, self.width,0) * 0.5
        vertex_4 = self.position + Coordinate(0,0,self.height)
        return np.array([vertex_0, vertex_1, vertex_2, vertex_3, vertex_4])

    def faces(self) -> np.ndarray:
        return np.array([[0,1,4],[1,2,4],[2,3,4],[3,0,4]])

    def type(self) -> SpikeForm:
        return SpikeForm.STANDARD
    @staticmethod
    def number_of_vertices() -> int:
        return 5

    @staticmethod
    def number_of_faces() -> int:
        return 4

class Surface:
    def __init__(self, surface_description: SurfaceDescriptionOptions):
        self.surface_description = surface_description

    def generate_surface_mesh(self):
        number = self.surface_description.spike_number
        width = self.surface_description.spike_width
        height = self.surface_description.spike_mean_height
        number_of_faces = PyramidVolume.number_of_faces()
        max_width_half = width * number * 0.5
        x = np.linspace(-max_width_half + width * 0.5, max_width_half - width * 0.5,number)
        y = x
        grid_x, grid_y = np.meshgrid(x, y)
        grid_points = np.column_stack([grid_x.ravel(), grid_y.ravel(), np.zeros(grid_x.size)])

        volume_vertices = PyramidVolume(height, width, Coordinate(0,0,0)).vertices()
        vertices = grid_points[None,:,None] + volume_vertices[None,:,:]
        vertices = vertices.reshape(-1,3)

        volume_faces = PyramidVolume(height, width, Coordinate(0,0,0)).faces()
        offset = (np.arange(len(grid_points)) * number_of_faces)[:,None,None]
        faces = volume_faces[None,:,:] + offset
        faces = faces.reshape(-1,3)
        print(vertices)
        print(faces)
        mesh = trimesh.Trimesh(vertices, faces)
        return mesh

    def surface_description(self):
        pass

    def calculate_mean_height(self):
        pass

    def calculate_root_mean_square_height(self):
        pass

    def calculate_skewness(self):
        pass

    def calculate_kurtosis(self):
        pass

    def calculate_all_surface_parameters(self):
        pass

    def __str__(self):
        pass

    def __repr__(self):
        pass

if __name__ == "__main__":
    description = SurfaceDescriptionOptions()
    description.spike_width = 1.
    description.spike_mean_height = 1.
    description.spike_layers = 1
    description.spike_number = 2
    description.spike_form = SpikeForm.STANDARD
    surface = Surface(description)
    mesh = surface.generate_surface_mesh()


    #mesh.show()
