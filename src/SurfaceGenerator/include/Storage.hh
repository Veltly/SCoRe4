// Copyright [2024] C.Gruener
// Date: 23-06-01
// File: VolumeDescription is used to pass information in Surface Generator
// between classes

#ifndef SRC_SURFACEGENERATOR_INCLUDE_STORAGE_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_STORAGE_HH_

#include <vector>

#include "G4Transform3D.hh"
#include "G4TriangularFacet.hh"
#include "G4Types.hh"

namespace Surface {

struct SolidDescription {
  enum class Solid { Box, Trd };
  Solid Volumetype;
  std::vector<G4double> Volumeparameter;
  G4Transform3D Transform;
  std::vector<G4int> OuterSurface;
};
}  // namespace Surface
#endif

#ifndef G4SURFACE_SURFACEDESCRIPTION
#define G4SURFACE_SURFACEDESCRIPTION
namespace Surface {
struct SurfaceDescription {
  G4int SpikeNr;
  G4TriangularFacet facet;
};
}  // namespace Surface
#endif  // SRC_SURFACEGENERATOR_INCLUDE_STORAGE_HH_
