//
//
//
//
//	author: C.Gruener
//
// VolumeDescription is used to pass information in Surface Generator between
// classes

#ifndef G4SURFACE_SOLIDDESCRIPTION
#define G4SURFACE_SOLIDDESCRIPTION

// #include "Printable.hh"
#include <G4Transform3D.hh>
#include <G4TriangularFacet.hh>
#include <G4Types.hh>
#include <vector>

namespace Surface {

struct SolidDescription {
  enum class Solid { Box, Trd };
  Solid Volumetype;
  std::vector<G4double> Volumeparameter;
  G4Transform3D Transform;
  std::vector<int> OuterSurface;
};
} // namespace Surface
#endif

#ifndef G4SURFACE_SURFACEDESCRIPTION
#define G4SURFACE_SURFACEDESCRIPTION
namespace Surface {
struct SurfaceDescription {
  int SpikeNr;
  G4TriangularFacet facet;
};
} // namespace Surface
#endif
