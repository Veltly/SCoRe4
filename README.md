# SCoRe4 - A Geant4-based library for Surface Contamination and Roughness effects Simulations

!!(Readme will still be updated with more description)

This library is used to simulate surface near particle tracks including a surface structure in micrometer range.
It is used to simulate a patch of rough surface structure based on defined parameters and simulate a large surface using the developed portation ansatz.
For this the library has different modules:

    - Surface Generator
    - Particle Generator
    - Portal
    - ParameterToSurface

## Surface Generator

Generates a patch of rough surface

## Particle Generator

Randomly samples starting vertices at the surface

## Portal

Implements the portation of particles to reuse a smaller simulated volume multiple times to fake a bigger volume.

## ParameterToSurface

Python extension to generate a random surface profile and exports it to a gdml file.

## Installation

```
git clone https://github.com/Veltly/G4Surface.git && 
cd G4Surface && mkdir build && cd build && cmake ..
make install
```
## Examples

Two examples are provided:

    - example_surface
    - example_surface_portal

### Example_Surface:
Description of example

### Example_Surface_Portal:
Description of example

## Used Libraries

- Numpy
- Matplotlib
- Scipy
- numba
- trimesh
- lxml
- pandas
- tqdm 
- yaml

## Contact:
In case of feedbacke, errors, suggestions or you want to contribute please contact Christoph Gruener (christoph.gruener@oeaw.ac.at)

## Citation

If you use this library in your work please reference it accordingly.

@article{grüner2025geant4basedlibraryscore4,
      title={Geant4 based library SCoRe4 for Surface Contamination and Roughness Effects simulations in rare event search experiments}, 
      author={Christoph Grüner},
      year={2025},
      eprint={2511.15844},
      archivePrefix={arXiv},
      primaryClass={physics.ins-det},
      url={https://arxiv.org/abs/2511.15844}, 
}
