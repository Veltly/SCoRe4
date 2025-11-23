# SCoRe4 - A Geant4-based library for Surface Contamination and Roughness effects Simulations

As this code is still under development, you can find the latest version on GitHub.\
https://github.com/Veltly/SCoRe4

This library is used to simulate surface near particle tracks including a surface structure in micrometer range.
It is used to simulate a patch of rough surface structure based on defined parameters and simulate a large surface using the developed portal ansatz.
Two different approaches are provided.
One is based purely on C++ and Geant4 and implements a rough surface using spike-like structures and extends the surface to larger surfaces using a portal subworld configuration.
The corresponding modules used can be found at:

    - src/SurfaceGenerator
    - src/ParticleGenerator
    - src/Portal

Further, helper classes can be found in src/Service.
An example of this implementation can be found in examples/example_surface_portal

The second approach is split between the generation of a surface using a python command line tool and the simulation in Geant4.
It enables significantly more complex surfaces, but is two orders of magnitude slower than the other approach and is still under development.

The corresponding modules can be found in:

    - src/ParameterToSurface
    - src/Surface

An example can be found in examples/example_surface

## Surface Generator

The surface generator creates a patch of rough surface represented by multiple spike-shaped structures.
A single structure can be either one physical volume or a combination of multiple physical volumes.
All spikes in a batch look the same to reduce the memory footprint of the object and increase simulation speed.

To generate the area, an auxiliary class “RoughnessHelper” is implemented, which is located in src/Service.
The class can be controlled via a macro file. A template can be found in mac/.
It is recommended to use only the auxiliary class to ensure correct setup.

## Portal

The key element for surface simulation with macroscopic areas is the Portal Subworld module.
This allows a larger volume to be represented by a much smaller sub-volume, which can be traversed multiple times by a particle interacting with the large volume.

To simplify setup, an auxiliary class Surface::MultiportalHelper is provided, which first collects all the necessary information, such as the
size of the portal, the size of the subworld, the number of different subworlds and their frequency, materials,
etc. After providing the information, the auxiliary class generates a portal-subworld pair.
Some options can be controlled via a macro file. A template is available in mac/.

To use the portal mechanism, it is important to add the DoStep(...) command from the Portal class to G4UserSteppingAction.

It is recommended to use only the auxiliary class to ensure correct setup.

## Particle Generator

The particle generator can evenly distribute points on the rough surface in relation to their placement in a portal subworld.
To do this, the generated rough surface must be linked to the portal.

It is recommended to use the auxiliary classes Surface::MultiportalHelper and Surface::RoughnessHelper for correct setup of the portal and rough surface.
The classes also link the surface and subworld to the particle generator, which can now be used to generate primary events.

For an example of setting up a rough surface using the portal, see examples/example_surface_portal.

## ParameterToSurface

This extension can be used to generate a random and significantly more complex surface profile for simulation.
In the current implementation, it can be used as follows:

- Use the Python command line tool to generate a more complex surface and export it to a GDML file. This file format can be read by Geant4.
- In Geant4, use the provided LogicalSurface class to generate a G4LogicalVolume from the GDML file.
- Use the provided SurfacePlacement class to place the logical surface volume as a physical volume.

Loading the volume with the provided class links the surface to the provided particle generator.

For an example, see examples/example_surface.

A list of required python libraries can be found in src/ParameterToSurface/requirements.txt

## Installation

```
git clone https://github.com/Veltly/SCoRe4.git && \
cd SCoRe4 && mkdir -p build && cd build && \
cmake .. -DCMAKE_INSTALL_PREFIX=<install_path> && \
make -j4 && make install
```

## Examples

Two examples are provided:

    - example_surface
    - example_surface_portal

After installing the library, both examples can be compiled and executed separately.
Both examples simulate a rough surface with structures in the micrometer range on a G4_SI crystal and particle contamination placed on the surface.
The simulation can be controlled using the macro files provided.

The simulations output two CSV-files.
One stores the energy introduced into the surface + crystal, the other stores the energy that has escaped.

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

## Contributions, Bugs, Suggestions:
If you have any feedback, problems, suggestions, or ideas and would like to contribute, please contact Christoph Grüner (christoph.gruener(at)oeaw.ac.at).
If you have found any errors or are experiencing problems with the current version, please create a bug report at https://github.com/Veltly/SCoRe4.
## Citation

If you use this library in your work, please cite the appropriate source.

@article{grüner2025geant4basedlibraryscore4,
      title={Geant4 based library SCoRe4 for Surface Contamination and Roughness Effects simulations in rare event search experiments}, 
      author={Christoph Grüner},
      year={2025},
      eprint={2511.15844},
      archivePrefix={arXiv},
      primaryClass={physics.ins-det},
      url={https://arxiv.org/abs/2511.15844}, 
}
