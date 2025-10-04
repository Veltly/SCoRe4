import numpy as np
from enum import Enum
import matplotlib.pyplot as plt

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
    def heightmap(self):
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

    def plot(self):
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
    def __str__(self):
        return str(self.heightmap)

if __name__ == "__main__":
    heightMap = HeightMap((10,10),(1.,1.))
    #heightMap.wave(frequency = 1., amplitude = 10., direction = HeightMap.Direction.X)
    heightMap.random()
    heightMap.plot()