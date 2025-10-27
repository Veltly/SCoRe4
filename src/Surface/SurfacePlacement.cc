/**
* @brief Definition of SurfacePlacement class
* @author C.Gruener
* @date 2025-10-24
* @file SurfacePlacement.cc
*/

#include "SurfacePlacement.hh"
#include "SurfaceSourceStore.hh"
#include "G4PVPlacement.hh"

namespace Surface {
SurfacePlacement::SurfacePlacement(G4RotationMatrix *rotation_matrix,
                                   const G4ThreeVector &position,
                                   LogicalSurface *logical_surface,
                                   const G4String &name,
                                   G4LogicalVolume *logical_mother_volume,
                                   G4bool check_overlaps,
                                   G4bool add_to_particle_generator) {
  if(add_to_particle_generator)
    add_to_surface_source_store(rotation_matrix,position, logical_surface);

  new G4PVPlacement(rotation_matrix,
                position,
                logical_surface->get_logical_handle(),
                name,
                logical_mother_volume,
                false,
                0,
                check_overlaps);

}

void SurfacePlacement::add_to_surface_source_store(G4RotationMatrix* rotation_matrix ,
                                                   const G4ThreeVector& position,
                                                   LogicalSurface* volume) {
  auto &store = SurfaceSourceStore::getInstance();
  store.add(rotation_matrix, position,volume);
}
}  // namespace Surface