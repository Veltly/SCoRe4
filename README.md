# SCoRe4 - A Geant4-based library for Surface Contamination and Roughness effects Simulations

As this code is further under development please report any found bugs.
!!(Readme will still be updated with more description)

This library is used to simulate surface near particle tracks including a surface structure in micrometer range.
It is used to simulate a patch of rough surface structure based on defined parameters and simulate a large surface using the developed portation ansatz.
Two different aprroaches are provided.
One that is purely C++ and Geant4 based implementing a rough surface by using spike-like structures and extending the surface to bigger surfaces using a portal-subworld setup.
The corresponding used modules can be found in:

    - src/SurfaceGenerator
    - src/ParticleGenerator
    - src/Portal

Further, helper classes can be found in src/Service.
An example of this implementation can be found in examples/example_surface_portal

The second approach is split between the generation of a surface using a python commandlinetool and the simulation in Geant4.
It allows for much more complex surfaces, however is slower by a factor of two in comparison to the other approach.

The corresponding modules can be found in:

    - src/ParameterToSurface
    - src/Surface

An example can be found in examples/example_surface

## Surface Generator

The surface generator creates a patch of rough surface represented by multiple spike-shaped structures.
A single structure can be either one physical volume or a combination of multiple physical volumes.
All spikes in a batch look alike to reduce the memory footprint of the object and increase simulation speed.

For generating the patch a helpe class "RoughnessHelper" is implemented and can be found in src/Service.
The class can be controlled by a macrofile. A template can be found in mac/.

## Portal

The key element for a surface simulation with macroscopic areas is the portal-subworld module.
This allows to represent a bigger volume by a much smaller subvolume which can be traversed multiple times by a particle interacting with the big volume.

To simplify its setup, a helper class Surace::MultiportalHelper is provided, which first gathers all the needed information, such as the
size of the portal, size of the subworld, number of different subworlds and their frequency, materials,
etc. After providing the information, the helper generates a portal-subworld pair.
Some options can be controlled via a macro file. A template is provided in mac/.

To use the portal mechanism it is important to add the DoStep(...) command from the portal class to the G4UserSteppingAction.

## Particle Generator

The particle generator can evenly distribute points on the rough surface with respect to its placement in a portal subworld.
For that, the generated rough surface must be linked to the portal.

It is recommendet to use the helper classes Surface::MultiportalHelper and Surface::RoughnessHelper for a proper portal-rough-surface setup.
The classes also link the surface and subworld to the particle generator which can now be used to generate primary events.

An example setup of a rough surface using the portal can be found in examples/example_surface_portal.

## ParameterToSurface

This extension can be used to generate a random and much more complex surface profile for simulation.
In the current implementation it can be used by following these steps:

    - Use the python commandline-tool to generate a more complex surface and export it to a gdml file. This file format can be read in by Geant4.
    - In Geant4 use the class LogicalSurface to generate a G4LogicalVolume from the gdml file.
    - Use the class SurfacePlacement to place the logical surface volume as a physical volume.

Loading the volume with the provided class links the surface to the provided particle generator.

An example can be found in examples/example_surface.

## Installation

```
git clone https://github.com/Veltly/SCoRe4.git && \
 cd SCoRe4 && mkdir build && cd build && cmake .. && \
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

## Used Libraries and Frameworks

- Geant4
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
