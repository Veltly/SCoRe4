import numpy as np
from numpy.typing import NDArray
from enum import Enum
import matplotlib.pyplot as plt
from scipy.signal import convolve2d
from dataclasses import dataclass
import random

@dataclass
class RandomComplexParams:
    point_cluster_rounds: int | None = None
    point_cluster_diameter: float | None = None
    length_cluster_rounds: int | None = None
    length_cluster_init_length: float | None = None
    length_cluster_width: float | None = None
    even_out_rounds: int | None = None
    edge_height: float | None = None
    max_height: float | None = None
    min_height: float | None = None
    seed: int | None = None

class HeightMap:
    class Direction(Enum):
        X = 0
        Y = 1

    def __init__(self, n : tuple[int,int], length : tuple[float,float] ) -> None:
        self.nx = n[0]
        self.ny = n[1]
        self.length_x = length[0]
        self.length_y = length[1]
        self.dx = length[0] / self.nx
        self.dy = length[1] / self.ny
        self._heightmap = None

    @property
    def heightmap(self) -> NDArray:
        if self._heightmap is None:
            raise ValueError("No height map generated!")
        return self._heightmap

    def wave(self,frequency : float, amplitude : float, direction : Direction = Direction.X ) -> None:
        if direction == HeightMap.Direction.X:
            x = np.linspace(0, self.length_x, self.nx)
            wave = amplitude * np.sin(2 * np.pi * frequency * x) + amplitude
            heightmap = wave[np.newaxis, :]
            heightmap = np.broadcast_to(heightmap, (self.ny, wave.size))
        elif direction == HeightMap.Direction.Y:
            y = np.linspace(0, self.length_y, self.ny)
            wave = amplitude * np.sin(2 * np.pi * frequency * y) + amplitude
            heightmap = wave[:, np.newaxis]
            heightmap = np.broadcast_to(heightmap, (wave.size, self.nx))
        else:
            raise ValueError("Invalid direction argument!")
        self._heightmap = np.array(heightmap)

    def random(self,height : float = 1., seed : int | None = None) -> None:
        if seed is not None:
            np.random.seed(seed)
        heightmap = np.random.random((self.ny, self.nx)) * height
        self._heightmap = heightmap

    def set_edge(self, height : float = 0.) -> None:
        heightmap = self.heightmap
        heightmap[0, :] = height
        heightmap[-1, :] = height
        heightmap[:, 0] = height
        heightmap[:, -1] = height
        self._heightmap = heightmap

    def even_out(self, edge_weight : float = 1., center_weight : float = 1., runs : int = 1) -> None:
        heightmap = self.heightmap
        kernel = np.array([
            [0, edge_weight, 0],
            [edge_weight, center_weight, edge_weight],
            [0, edge_weight, 0],
        ])
        kernel = kernel / np.sum(kernel)
        for _ in range(runs):
            heightmap = convolve2d(heightmap, kernel, mode='same')
        self._heightmap = heightmap

    def add_heights(self,heights : float, samples : int) -> None:
        col = np.random.randint(0,self.nx, size=samples)
        row = np.random.randint(0,self.ny, size=samples)
        heightmap = self.heightmap
        heightmap[row,col] = heights
        self._heightmap = heightmap

    def random_connected_cluster(self, size : int, initial_points : NDArray[tuple[int,int]] | None = None) -> NDArray[np.int_]:
        cols = self.nx
        rows = self.ny
        if initial_points is None:
            initial_points = (np.random.randint(0, rows), np.random.randint(0, cols))
            initial_points = np.array([initial_points])
        outer_edge = self._find_outer_edge(initial_points)
        visited = set(map(tuple,initial_points))

        def add_neighbors(y:int,x:int) -> None:
            for ny, nx in [(y, x - 1), (y, x + 1), (y - 1, x), (y + 1, x)]:
                if 0 <= ny < rows and 0 <= nx < cols and (nx, ny) not in visited:
                    outer_edge.add((ny, nx))

        for point in initial_points:
            add_neighbors(*point)

        while len(visited) < size:
            if not outer_edge:
                break  # no more expansion possible
            new_point = random.sample(tuple(outer_edge), 1)[0]
            outer_edge.remove(new_point)
            add_neighbors(*new_point)
            visited.add(new_point)

        mask = np.zeros((rows,cols), dtype=np.bool)
        cluster = np.array(list(visited))
        mask[cluster[:,0], cluster[:,1]] = True
        return mask

    def set_height(self, height : float , mask : np.ndarray[np.dtype[bool]]) -> None:
        heightmap = self.heightmap
        heightmap[mask] = height
        self._heightmap = heightmap

    def set_cluster(self, height : float, cluster_size : int, rounds : int) -> None:
        heightmap = self.heightmap
        for _ in range(rounds):
            mask = self.random_connected_cluster(cluster_size)
            heightmap[mask] = height
        self._heightmap = heightmap


    def set_length_cluster_physical(self, height : float, width : float, initial_length_physical : float, rounds : int ) -> None:
        heightmap = self.heightmap
        density = self.nx * self.ny / (self.length_y * self.length_x)
        dy = self.length_y / self.ny
        dx = self.length_x / self.nx
        for _ in range(rounds):
            direction = (random.uniform(-1,1), random.uniform(-1,1))
            start = (np.random.randint(0,self.ny), np.random.randint(0,self.nx))
            start_line = self._physical_line_to_grid(start=start, direction=direction, length=initial_length_physical)
            end = start_line[-1]
            actual_length = np.linalg.norm([(end[1] - start[1]) * dy, (end[0] - start[0]) * dx])
            cluster_size = int(width * actual_length * density)
            mask = self.random_connected_cluster(size=cluster_size, initial_points=start_line)
            heightmap[mask] = height
        self._heightmap = heightmap

    def _physical_line_to_grid(self, start : tuple[int,int],length : float, direction : tuple[float,float]) -> NDArray[np.int_]:
        start_y, start_x = start
        direction = np.array(direction)
        direction = direction / np.linalg.norm(direction)
        dx = direction[1]
        dy = direction[0]
        step_x = 1 if direction[1] > 0 else -1
        step_y = 1 if direction[0] > 0 else -1

        x0 = start_x
        y0 = start_y

        t_max_x = ((x0 + (step_x > 0)) - x0) / dx if dx != 0 else float('inf')
        t_max_y = ((y0 + (step_y > 0)) - y0) / dy if dy != 0 else float('inf')

        # Distance between subsequent crossings
        t_delta_x = abs(1 / dx) if dx != 0 else float('inf')
        t_delta_y = abs(1 / dy) if dy != 0 else float('inf')

        points = [(y0, x0)]

        x, y = x0, y0
        current_length = 0
        delta_x = 0
        delta_y = 0
        while current_length < length:
            # Step in whichever direction is closer
            if t_max_x < t_max_y:
                x += step_x
                t_max_x += t_delta_x
                delta_x += self.dx
            elif t_max_y < t_max_x:
                y += step_y
                t_max_y += t_delta_y
                delta_y += self.dy
            else:
                x += step_x
                y += step_y
                t_max_x += t_delta_x
                t_max_y += t_delta_y
                delta_x += self.dx
                delta_y += self.dy

            if x < 0 or x >= self.nx:
                break
            if y < 0 or y >= self.ny:
                break
            points.append((y, x))
            current_length = np.linalg.norm(np.array([delta_y, delta_x]))
        return np.array(points, dtype=int)

    def _find_outer_edge(self, points : NDArray[tuple[int,int]]) -> set[tuple[int,int]]:
        cols = self.nx
        rows = self.ny
        outer_edge = set()
        for y, x in points:
            for ny, nx in [(y,x-1),(y,x+1),(y-1,x),(y+1,x)]:
                if 0 <= ny < rows and 0 <= nx < cols and not np.any(np.all(points == (ny,nx), axis=1)):
                    outer_edge.add((ny, nx))
        return outer_edge

    def plot(self) -> None:
        plt.imshow(self.heightmap,
           origin='lower',   # so y=0 is at bottom
           extent=(0., self.length_x, 0., self.length_y),
           cmap='viridis',   # color map
           aspect='auto')    # make axes proportional
        plt.colorbar(label='Height')
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.title('Height Map')
        plt.show()

    def __str__(self) -> str:
        return str(self.heightmap)

    def random_complex(self, cluster_rounds: int, cluster_diameter : float, max_height : float, min_height: float, seed : int | None = None) -> None:
        density = (self.nx * self.ny) / (self.length_x * self.length_y)
        cluster_size = int(cluster_diameter**2 * np.pi * density / 4.)
        self.random(seed=seed)
        for _ in range(cluster_rounds):
            self.set_cluster(max_height,cluster_size,1)
            self.set_cluster(min_height,cluster_size,1)
        self.even_out(1,1,10)
        self.set_edge((min_height + max_height) * 0.5)

    def random_complex_all(self, params : RandomComplexParams = RandomComplexParams()) -> None:
        self.random(seed=params.seed)

        if params.point_cluster_diameter is not None and params.point_cluster_rounds is not None and params.min_height is not None and params.max_height is not None:
            density = (self.nx * self.ny) / (self.length_x * self.length_y)
            cluster_size = int(params.point_cluster_diameter ** 2 * np.pi * density / 4.)
            for _ in range(params.point_cluster_rounds):
                self.set_cluster(height=params.max_height,cluster_size=cluster_size, rounds=1)
                self.set_cluster(height=params.min_height,cluster_size=cluster_size,rounds=1)

        if params.length_cluster_rounds is not None and params.length_cluster_width is not None and\
                params.length_cluster_init_length is not None and \
                params.max_height is not None and params.min_height is not None:
            for _ in range(params.length_cluster_rounds):
                self.set_length_cluster_physical(height=params.max_height,
                                                 initial_length_physical=params.length_cluster_init_length,
                                                 width=params.length_cluster_width,
                                                 rounds=1)
                self.set_length_cluster_physical(height=params.min_height,
                                                initial_length_physical=params.length_cluster_init_length,
                                                 width=params.length_cluster_width,
                                                 rounds=1)
        if params.even_out_rounds is not None:
            self.even_out(1,1,runs=params.even_out_rounds)

        if params.edge_height is not None:
            self.set_edge(params.edge_height)

if __name__ == "__main__":
    heightMap = HeightMap((100,100),(10.,10.))
    #heightMap.wave(frequency = 1., amplitude = 10., direction = HeightMap.Direction.X)
    #heightMap.random_complex(cluster_rounds=1,cluster_diameter=10.,max_height=6,min_height=1)
    rc_params = RandomComplexParams()
    #params.cluster_rounds = 0
    #rc_params.cluster_diameter = 10.
    rc_params.length_cluster_rounds = 100
    rc_params.length_cluster_init_length = 3
    rc_params.length_cluster_width = 1.
    rc_params.even_out_rounds = 2
    #rc_params.edge_height = 3.
    rc_params.max_height = 6.
    rc_params.min_height = 0.
    rc_params.seed = None
    heightMap.random_complex_all(rc_params)
    heightMap.plot()