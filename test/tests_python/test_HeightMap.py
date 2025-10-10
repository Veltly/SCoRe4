from src.ParameterToSurface.HeightMap import HeightMap
import numpy as np
import pytest

@pytest.fixture
def heightmap():
    # Create a reasonably sized grid with physical dimensions
    return HeightMap(n=(100, 100), length=(10.0, 10.0))

@pytest.mark.parametrize(
    "start,length,direction,expected_last",
    [
        ((50,50), 2.0, (0.0,1.0),(50,70)),
        ((10,10), 2.5, (1.0,0.0),(35,10)),
        ((0,0), 5.0, (1.,1.),(36,36)),
        ((70,60),0.5,(-1.0,-1.5),(67,56))
    ]
)
def test_physical_line_to_grid_physical_returns_valid_points(heightmap, start, length, direction, expected_last):
    points = heightmap._physical_line_to_grid(start=start, direction=direction, length=length)

    # 1. Should return a non-empty list
    assert isinstance(points, list)
    assert len(points) > 0

    # 2. Points should be tuples of ints
    assert all(isinstance(p, tuple) and len(p) == 2 for p in points)
    assert all(isinstance(p[0], (int, np.integer)) and isinstance(p[1], (int, np.integer)) for p in points)

    assert points[0] == start
    assert points[-1] == expected_last

def test_find_outer_edge(heightmap):
    points = np.array([(84,17),(87, 19),(87,20)])
    result = {(83,17),(85,17),(84,16),(84,18),(87,18),(87,21),(88,19),(88,20),(86,19),(86,20)}
    outer_edge = heightmap._find_outer_edge(points)
    missing = result - outer_edge
    extra = outer_edge - result
    assert len(outer_edge) == 10
    assert not missing
    assert not extra