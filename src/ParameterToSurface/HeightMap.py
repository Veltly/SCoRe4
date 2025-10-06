import numpy as np
from enum import Enum
import matplotlib.pyplot as plt
from scipy.signal import convolve2d

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
    def heightmap(self) -> np.ndarray:
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

    def random_connected_cluster(self, size : int):
        cols = self.nx
        rows = self.ny
        start = (np.random.randint(0,rows), np.random.randint(0,cols))
        outer_edge = set()
        visited = set()
        visited.add(start)
        y, x = start
        for ny, nx in [(y,x-1),(y,x+1),(y-1,x),(y+1,x)]:
            if 0 <= ny < rows and 0 <= nx < cols and (nx, ny) not in visited:
                outer_edge.add((ny, nx))
        while len(visited) < size:
            if not outer_edge:
                break  # no more expansion possible
            new_point = list(outer_edge)[np.random.randint(0,len(outer_edge))]
            y, x = new_point
            for ny, nx in [(y,x-1),(y,x+1),(y-1,x),(y+1,x)]:
                if 0 <= ny < rows and 0 <= nx < cols and (nx, ny) not in visited:
                    outer_edge.add((ny, nx))
            outer_edge.remove(new_point)
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

    def set_length_cluster(self, height : float, length : int, rounds : int) -> None:
        heightmap = self.heightmap
        for _ in range(rounds):
            mask = self.random_connected_cluster_length(length)
            heightmap[mask] = height
        self._heightmap = heightmap
    def _random_line_to_grid(self,length : int, direction : tuple[float,float]):
        direction = np.array(direction)
        direction = direction / np.linalg.norm(direction)
        dx = direction[0]
        dy = direction[1]
        cols = self.nx
        rows = self.ny
        step_x = 1 if direction[0] > 0 else -1
        step_y = 1 if direction[1] > 0 else -1

        start_x, start_y = (np.random.randint(0,rows), np.random.randint(0,cols))
        x0 = start_x
        y0 = start_y

        t_max_x = ((x0 + (step_x > 0)) - x0) / dx if dx != 0 else float('inf')
        t_max_y = ((y0 + (step_y > 0)) - y0) / dy if dy != 0 else float('inf')

        # Distance between subsequent crossings
        t_delta_x = abs(1 / dx) if dx != 0 else float('inf')
        t_delta_y = abs(1 / dy) if dy != 0 else float('inf')

        points = [(y0, x0)]

        x, y = x0, y0
        for _ in range(length):
            # Step in whichever direction is closer
            if t_max_x < t_max_y:
                x += step_x
                t_max_x += t_delta_x
            else:
                y += step_y
                t_max_y += t_delta_y
            if x < 0 or x >= self.nx:
                break
            if y < 0 or y >= self.ny:
                break
            points.append((y, x))
        return points

    def _find_outer_edge(self, points : np.ndarray) -> np.ndarray:
        cols = self.nx
        rows = self.ny
        outer_edge = set()
        for point in points:
            y, x = point
            for ny, nx in [(y,x-1),(y,x+1),(y-1,x),(y+1,x)]:
                if 0 <= ny < rows and 0 <= nx < cols and (nx, ny) not in points:
                    outer_edge.add((ny, nx))
        return outer_edge
    def random_connected_cluster_length(self, cluster_size : int, initial_length : int, direction : tuple[float, float] | None = None) -> np.ndarray:
        if direction is None:
            direction = (np.random.random(), np.random.random())
        cols = self.nx
        rows = self.ny
        visited = set(self._random_line_to_grid(initial_length, direction))
        outer_edge = set(self._find_outer_edge(np.array(list(visited))))
        print(visited)
        while len(visited) < cluster_size:
            if not outer_edge:
                break  # no more expansion possible
            new_point = list(outer_edge)[np.random.randint(0,len(outer_edge))]
            y, x = new_point
            for ny, nx in [(y,x-1),(y,x+1),(y-1,x),(y+1,x)]:
                if 0 <= ny < rows and 0 <= nx < cols and (nx, ny) not in visited:
                    outer_edge.add((ny, nx))
            outer_edge.remove(new_point)
            visited.add(new_point)

        mask = np.zeros((rows,cols), dtype=np.bool)
        cluster = np.array(list(visited))
        print(cluster)
        mask[cluster[:,0], cluster[:,1]] = True
        return mask



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

    def random_complex(self, cluster_rounds: int, cluster_diameter : float, max_height : float, min_height: float, seed : int | None = None):
        density = (self.nx * self.ny) / (self.length_x * self.length_y)
        cluster_size = cluster_diameter**2 * np.arctan(1.) * density
        self.random(seed=seed)
        for _ in range(cluster_rounds):
            self.set_length_cluster(max_height,cluster_size,1)
            self.set_length_cluster(min_height,cluster_size,1)
        self.even_out(1,1,10)
        self.set_edge((min_height + max_height) * 0.5)


if __name__ == "__main__":
    heightMap = HeightMap((10,10),(100.,100.))
    #heightMap.wave(frequency = 1., amplitude = 10., direction = HeightMap.Direction.X)
    heightMap.random_complex(cluster_rounds=1,cluster_diameter=10.,max_height=6,min_height=1)
    heightMap.plot()