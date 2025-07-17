# G4Surface

This library is used to simulate surface near particle tracks including a surface structure in micrometer range.
It is used to simulate a patch of rough surface structure based on defined parameters and simulate a large surface using the developed portation ansatz.
For this the library has different modules:

    - Surface Generator
    - Particle Generator
    - Portal

## Surface Generator

Generates a patch of rough surface

## Particle Generator

Randomly samples starting vertices at the surface

## Portal

Implements the portation of particles to reuse a smaller simulated volume multiple times to fake a bigger volume.

## Installation

```
git clone https://github.com/Veltly/G4Surface.git && 
cd G4Surface && mkdir build && cd build && cmake ..
make install
```

## Citation

If you use this library in your work please reference it accordingly.

**Add citation of code release paper here**