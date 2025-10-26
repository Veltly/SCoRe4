/**
 * @brief Definition of SurfacePlacement class
 * @author C.Gruener
 * @date 2025-10-24
 * @file SurfacePlacement.hh
 */

#ifndef SURFACE_SURFACEPLACEMENT_HH
#define SURFACE_SURFACEPLACEMENT_HH

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "Surface/LogicalSurface.hh"

namespace Surface {
/**
 * @brief Class SurfacePlacement is equivalent to G4PVPlacement.
 * @details It places a LogicalSurface object in simulation.
 * Further can add the object to the surface particle generator.
 */
class SurfacePlacement {
 public:
  SurfacePlacement(G4RotationMatrix *rotation_matrix,
                   const G4ThreeVector &position,
                   Surface::LogicalSurface &logical_surface,
                   const G4String &name,
                   G4LogicalVolume *logical_mother_volume,
                   G4bool check_overlaps,
                   G4bool add_to_particle_generator
                   );

  SurfacePlacement(G4RotationMatrix *rotation_matrix,
                   const G4ThreeVector &position,
                   G4LogicalVolume *logical_surface,
                   const G4String &name,
                   G4LogicalVolume *logical_mother_volume,
                   G4bool check_overlaps,
                   G4bool add_to_particle_generator
  );

 private:
  static void add_to_surface_source_store(G4RotationMatrix*,
                                          const G4ThreeVector &,
                                          G4LogicalVolume* volume);
};

}  // namespace Surface

#endif  // SURFACE_SURFACEPLACEMENT_HH
