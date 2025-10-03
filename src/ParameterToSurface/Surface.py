from dataclasses import dataclass
from enum import Enum
from abc import ABC, abstractmethod
from line_profiler import LineProfiler
from numba import njit

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
        self.mesh = self.generate_surface_mesh()
        self._mean_height = None
        self._root_mean_square_height = None
        self._skewness = None
        self._kurtosis = None
        self._base_area = None

    @property
    def mean_height(self) -> float:
        if self._mean_height is None:
            self._mean_height = self.calculate_mean_height()
        return self._mean_height

    @property
    def root_mean_square_height(self) -> float:
        if self._root_mean_square_height is None:
            self._root_mean_square_height = self.calculate_root_mean_square_height()
        return self._root_mean_square_height
    @property
    def skewness(self) -> float:
        if self._skewness is None:
            self._skewness = self.calculate_skewness()
        return self._skewness
    @property
    def kurtosis(self) -> float:
        if self._kurtosis is None:
            self._kurtosis = self.calculate_kurtosis()
        return self._kurtosis

    @property
    def base_area(self) -> float:
        if self._base_area is None:
            self._base_area = self.calculate_base_area()
        return self._base_area

    def generate_surface_mesh(self):
        number = self.surface_description.spike_number
        width = self.surface_description.spike_width
        height = self.surface_description.spike_mean_height
        number_of_vertices = PyramidVolume.number_of_vertices()
        max_width_half = width * number * 0.5
        #generate grid for placing volumes
        x = np.linspace(-max_width_half + width * 0.5, max_width_half - width * 0.5,number)
        y = x
        grid_x, grid_y = np.meshgrid(x, y)
        grid_points = np.column_stack([grid_x.ravel(), grid_y.ravel(), np.zeros(grid_x.size)])
        #create list of all vertices
        volume_vertices = PyramidVolume(height, width, Coordinate(0,0,0)).vertices()
        vertices = grid_points[None,:,None] + volume_vertices[None,:,:]
        vertices = vertices.reshape(-1,3)
        #create list of all faces
        volume_faces = PyramidVolume(height, width, Coordinate(0,0,0)).faces()
        offset = (np.arange(len(grid_points)) * number_of_vertices)[:,None,None]
        faces = volume_faces[None,:,:] + offset
        faces = faces.reshape(-1,3)
        #return mesh
        return trimesh.Trimesh(vertices, faces)

    def height_map(self,nx:int, ny:int):
        x_min, y_min, z_min = self.mesh.bounds[0]
        x_max, y_max, z_max = self.mesh.bounds[1]
        x = np.linspace(x_min, x_max, nx)
        y = np.linspace(y_min, y_max, ny)
        xx, yy = np.meshgrid(x, y)
        ray_origin = np.column_stack([xx.ravel(), yy.ravel(), np.full(xx.size, z_max + 1)])
        #print(ray_origin)
        ray_direction = np.tile([0,0,-1], (ray_origin.shape[0],1))
        #print(ray_direction)
        locations, index_ray, index_tri = self.mesh.ray.intersects_location(ray_origins=ray_origin,ray_directions=ray_direction)
        height_map = np.full(xx.shape, np.nan)
        height_map.ravel()[index_ray] = locations[:, 2]  # z-coordinate
        return height_map

    @staticmethod
    def facet_mean_height(v1: np.ndarray, v2:np.ndarray, v3:np.ndarray) -> float:
        area = np.linalg.norm(np.cross(v2 - v1, v3 - v1)) * 0.5
        integral = area * (v1[2] + v2[2] + v3[2]) / 3.0
        return integral

    def surface_description(self):
        pass

    @staticmethod
    @njit
    def _mean_height_numba(vertices, faces):
        total = 0.0
        for i in range(faces.shape[0]):
            f = faces[i]
            v0 = vertices[f[0]]
            v1 = vertices[f[1]]
            v2 = vertices[f[2]]
            jacobi = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0])
            jacobi = abs(jacobi)
            mean_z = (v0[2] + v1[2] + v2[2]) / 6.0
            total += mean_z * jacobi
        return total
    def calculate_mean_height(self):
        x_min, y_min, _ = self.mesh.bounds[0]
        x_max, y_max, _ = self.mesh.bounds[1]
        base_area = (x_max - x_min) * (y_max - y_min)
        return self._mean_height_numba(self.mesh.vertices, self.mesh.faces) / base_area

    @staticmethod
    @njit
    def _root_mean_height_squared_numba(vertices, faces, mean_height):
        total = 0.0
        for i in range(faces.shape[0]):
            f = faces[i]
            v0 = vertices[f[0]]
            v1 = vertices[f[1]]
            v2 = vertices[f[2]]
            z0 = v0[2] - mean_height
            z1 = v1[2] - mean_height
            z2 = v2[2] - mean_height
            jacobi = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0])
            jacobi = abs(jacobi)
            mean_z = (z0 * z0 + z1 * z1 + z2 * z2 + z0 * z1 + z1 * z2 + z2 * z0) / 12.
            total += mean_z * jacobi
        return total

    def calculate_base_area(self) -> float:
        x_min, y_min, _ = self.mesh.bounds[0]
        x_max, y_max, _ = self.mesh.bounds[1]
        base_area = (x_max - x_min) * (y_max - y_min)
        return base_area
    def calculate_root_mean_square_height(self) -> float:
        base_area = self.base_area
        mean_height = self.mean_height
        return np.sqrt(self._root_mean_height_squared_numba(self.mesh.vertices, self.mesh.faces, mean_height) / base_area)

    def calculate_skewness(self):
        base_area = self.base_area
        mean_height = self.mean_height
        root_mean_square_height = self.root_mean_square_height
        return self._skewness_numba(self.mesh.vertices, self.mesh.faces, mean_height) / base_area / root_mean_square_height**3

    @staticmethod
    @njit
    def _skewness_numba(vertices, faces, mean_height):
        total = 0.0
        for i in range(faces.shape[0]):
            f = faces[i]
            v0 = vertices[f[0]]
            v1 = vertices[f[1]]
            v2 = vertices[f[2]]
            z0 = v0[2] - mean_height
            z1 = v1[2] - mean_height
            z2 = v2[2] - mean_height
            u = z0**3 + z1**3 + z2**3
            v = z0**2 * (z1 + z2) + z1**2 * (z0 + z2) + z2**2 * (z0 + z1)
            w = z0*z1*z2
            jacobi = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0])
            jacobi = abs(jacobi)
            mean_z = (u + v + w) / 20.
            total += mean_z * jacobi
        return total

    def calculate_kurtosis(self):
        base_area = self.base_area
        mean_height = self.mean_height
        root_mean_square_height = self.root_mean_square_height
        return self._kurtosis_numba(self.mesh.vertices, self.mesh.faces, mean_height) / base_area / root_mean_square_height**4


    @staticmethod
    @njit
    def _kurtosis_numba(vertices, faces, mean_height):
        total = 0.0
        for i in range(faces.shape[0]):
            f = faces[i]
            v0 = vertices[f[0]]
            v1 = vertices[f[1]]
            v2 = vertices[f[2]]
            z0 = v0[2] - mean_height
            z1 = v1[2] - mean_height
            z2 = v2[2] - mean_height

            z0_2 = z0**2
            z0_3 = z0**3
            z0_4 = z0**4

            z1_2 = z1**2
            z1_3 = z1**3
            z1_4 = z1**4

            z2_2 = z2**2
            z2_3 = z2**3
            z2_4 = z2**4

            a = z0_4 + z1_4 + z2_4
            b = z0_3 * z1 + z0_3 * z2 + z1_3 * z0 + z1_3 * z2 + z2_3 * z0 + z2_3 * z1
            c = z0_2 * z1_2 + z1_2 * z2_2 + z0_2 * z2_2
            d = z0_2 * z1 *z2 + z1_2 *z0 * z2 + z2_2 *z1 * z0
            jacobi = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0])
            jacobi = abs(jacobi)
            mean_z = (a + b + c + d) / 30.
            total += mean_z * jacobi
        return total

    def __str__(self):
        pass

    def __repr__(self):
        pass

    def test_calculation(self, samples = 1000, plot = False):
        import surfalize
        height_map = surface.height_map(samples,samples)
        dx = description.spike_width * description.spike_number / samples
        dy = dx
        test_surface = surfalize.Surface(height_map, dx, dy)
        test_sv, test_sq, test_ssk, test_sku = test_surface.roughness_parameters(['Sv', 'Sq', 'Ssk','Sku']).values()
        diff_sv = test_sv - self.mean_height
        diff_sq = test_sq - self.root_mean_square_height
        diff_ssk = test_ssk - self.skewness
        diff_sku = test_sku - self.kurtosis
        print()
        print(f'-------------------------------------------------------')
        print(f'Comparison of surface parameters with library surfalize')
        print(f'Use {samples} x {samples} samples to translate mesh to surfalize representation')
        print(f'Area in x-y plane: {self.base_area} um^2')
        print(f'dx = {dx} um, dy = {dy} um')
        print(f'-------------------------------------------------------')
        print(f'         Test :   Real   : Difference : Relative Difference [%]')
        print(f' Sv: {test_sv: .5f} : {self.mean_height: .5f} : {diff_sv: .3e} : {diff_sv / self.mean_height * 100.: .2f}')
        print(f' Sq: {test_sq: .5f} : {self.root_mean_square_height: .5f} : {diff_sq: .3e} : {diff_sq / self.root_mean_square_height * 100.: .2f}')
        print(f'Ssk: {test_ssk: .5f} : {self.skewness: .5f} : {diff_ssk: .3e} : {diff_ssk / self.skewness * 100.: .2f}')
        print(f'Sku: {test_sku: .5f} : {self.kurtosis: .5f} : {diff_sku: .3e} : {diff_sku / self.kurtosis * 100: .2f}')
        print(f'-------------------------------------------------------')

        if plot:
            test_surface.show()


if __name__ == "__main__":
    description = SurfaceDescriptionOptions()
    description.spike_width = 1.
    description.spike_mean_height = 9.3
    description.spike_layers = 1
    description.spike_number = 100
    description.spike_form = SpikeForm.STANDARD
    surface = Surface(description)
    surface.test_calculation(1000,False)
