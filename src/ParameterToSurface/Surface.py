from dataclasses import dataclass
from enum import Enum
from abc import ABC, abstractmethod
from numba import njit
import numpy as np
import trimesh

from src.ParameterToSurface.HeightMap import HeightMap


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
    class Source(Enum):
        DESCRIPTION = 0
        HEIGHTMAP = 1

    def __init__(self, **kwargs ) -> None:
        self._mean_height = None
        self._root_mean_square_height = None
        self._skewness = None
        self._kurtosis = None
        self._base_area = None
        self._dx = None
        self._dy = None
        self._length_x = None
        self._length_y = None
        self._nx = None
        self._ny = None
        self._src = None
        self._surface_description = None
        if "description" in kwargs:
            self._src = "description"
            surface_description = kwargs.get("description")
            self.mesh = self.generate_surface_mesh_from_description(surface_description)
        elif "heightmap" in kwargs:
            self._src = "heightmap"
            surface_height = kwargs.get("heightmap")
            self.mesh = self.generate_surface_mesh_from_height_map(surface_height)
        else:
            raise ValueError("Require kwarg `description` or `heightmap`")

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

    @property
    def dx(self) -> float:
        if self._dx is None:
            raise ValueError("Value dx not set!")
        return self._dx

    @property
    def dy(self) -> float:
        if self._dy is None:
            raise ValueError("Value dy not set!")
        return self._dy

    @property
    def length_x(self) -> float:
        if self._length_x is None:
            raise ValueError("Value length_x not set!")
        return self._length_x

    @property
    def length_y(self) -> float:
        if self._length_y is None:
            raise ValueError("Value length_y not set!")
        return self._length_y

    @property
    def nx(self) -> int:
        if self._nx is None:
            raise ValueError("Value nx not set!")
        return self._nx

    @property
    def ny(self) -> int:
        if self._ny is None:
            raise ValueError("Value ny not set!")
        return self._ny

    @property
    def source(self) -> str:
        if self._src is None:
            raise ValueError("Value source not set!")
        return self._src

    @property
    def surface_description(self) -> SurfaceDescriptionOptions:
        if self._surface_description is None:
            raise ValueError("Value surface_description not set!")
        return self._surface_description

    def generate_surface_mesh_from_description(self, surface_description) -> trimesh.Trimesh:
        number = surface_description.spike_number
        width = surface_description.spike_width
        self._length_x = number * width
        self._length_y = number * width
        self._surface_description = surface_description
        height = surface_description.spike_mean_height
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

    def generate_surface_mesh_from_height_map(self, surface_height) -> trimesh.Trimesh:
        nx = surface_height.nx
        ny = surface_height.ny
        self._nx = nx
        self._ny = ny
        length_x = surface_height.length_x
        length_y = surface_height.length_y
        self._length_x = length_x
        self._length_y = length_y
        x = np.linspace(-length_x * 0.5, length_x * 0.5, nx)
        y = np.linspace(-length_y * 0.5, length_y * 0.5, ny)
        self._dx = length_x / nx
        self._dy = length_y / ny
        grid_x, grid_y = np.meshgrid(x, y)
        heightmap = surface_height.heightmap
        vertices = np.column_stack([grid_x.ravel(), grid_y.ravel(), heightmap.reshape(-1)])

        faces = np.empty((2 * (nx - 1) * (ny - 1), 3), dtype=int)
        i, j = np.meshgrid(np.arange(nx - 1), np.arange(ny - 1), indexing='xy')
        base = i + j * nx
        base = base.ravel()
        faces[0::2, :] = np.stack([base, base + 1, base + 1 + nx], axis=1)
        faces[1::2, :] = np.stack([base, base + 1 + nx, base + nx], axis=1)
        mesh = trimesh.Trimesh(vertices, faces)
        return mesh

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
        area = 0.5 * np.linalg.norm(np.cross(v2 - v1, v3 - v1))
        integral = area * (v1[2] + v2[2] + v3[2]) / 3.0
        return integral

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
        base_info = (
            f"Surface ({self.source})\n"
            f"  Length: x={self.length_x:.3f}, y={self.length_y:.3f};\n"
            f"  Base area: {self.base_area:.3f}\n"
        )

        if self.source == "description":
            extra = (
                f"  Number of Spikes: {self.surface_description.spike_number * self.surface_description.spike_number}\n"
                f"  Spike  width: {self.surface_description.spike_width:.3f}\n"
                f"  Spike mean height: {self.surface_description.spike_mean_height:.3f}\n"
            )
        elif self.source == "heightmap":
            extra = (
                f"  Number of grid points: x={self.nx},  y={self.ny}\n"
                f"  Resolution: dx={self.dx:.3e},  dy={self.dy:.3e}\n"
            )
        else:
            extra = ()

        calculations = (
            f"  Sv:  {self.mean_height: .3f}\n"
            f"  Sq:  {self.root_mean_square_height: .3f}\n"
            f"  Sku: {self.kurtosis: .3f}\n"
            f"  Skk: {self.skewness: .3f}\n"
        )
        return base_info + extra + calculations

    def __repr__(self):
        return (f"<Surface(source={self.source}, "
                f"length_x={self.length_x}, length_y={self.length_y})>")

    def show(self):
        self.mesh.show()

    def test_calculation(self, samples = 1000, plot = False):
        import surfalize
        height_map = self.height_map(samples,samples)
        dx = self.length_x / samples
        dy = self.length_y / samples
        test_surface = surfalize.Surface(height_map, dx, dy)
        test_sv, test_sq, test_ssk, test_sku = test_surface.roughness_parameters(['Sv', 'Sq', 'Ssk','Sku']).values()
        diff_sv = test_sv - self.mean_height
        diff_sq = test_sq - self.root_mean_square_height
        diff_ssk = test_ssk - self.skewness
        diff_sku = test_sku - self.kurtosis
        print(
            f"\n"
            f"-------------------------------------------------------\n"
            f"Comparison of surface parameters with library surfalize\n"
            f"Use {samples} x {samples} samples to translate mesh to surfalize representation\n"
            f"Area in x-y plane: {self.base_area} um^2\n"
            f"dx = {dx} um, dy = {dy} um\n"
            f"-------------------------------------------------------\n"
            f"         Test :   Real   : Difference : Relative Difference [%]\n"
            f" Sv: {test_sv: .5f} : {self.mean_height: .5f} : {diff_sv: .3e} : {diff_sv / self.mean_height * 100.: .2f}\n"
            f" Sq: {test_sq: .5f} : {self.root_mean_square_height: .5f} : {diff_sq: .3e} : {diff_sq / self.root_mean_square_height * 100.: .2f}\n"
            f"Ssk: {test_ssk: .5f} : {self.skewness: .5f} : {diff_ssk: .3e} : {diff_ssk / self.skewness * 100.: .2f}\n"
            f"Sku: {test_sku: .5f} : {self.kurtosis: .5f} : {diff_sku: .3e} : {diff_sku / self.kurtosis * 100: .2f}\n"
            f"-------------------------------------------------------\n")

        if plot:
            test_surface.show()


def test_height_map_wave():
    height_map = HeightMap((3000,3000), (1.,1.))
    height_map.wave(1.,0.2)
    surface = Surface(heightmap=height_map)
    print(surface)
    #surface.show()
    surface.test_calculation(100,False)

def test_height_map_random():
    height_map = HeightMap((10,10), (1.,1.))
    height_map.random(0.1)
    height_map.even_edge(0.05)
    surface = Surface(heightmap=height_map)
    print(surface)
    #surface.show()
    surface.test_calculation(1000,True)

def test_surface_description():
    description = SurfaceDescriptionOptions()
    description.spike_width = 1.
    description.spike_mean_height = 9.3
    description.spike_layers = 1
    description.spike_number = 100
    description.spike_form = SpikeForm.STANDARD
    surface = Surface(description=description)
    print(surface)
    #surface.show()
    surface.test_calculation(1000,False)


if __name__ == "__main__":
    #test_surface_description()
    #test_height_map_wave()
    test_height_map_random()
