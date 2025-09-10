/**
 * @brief Definition of structs to describe a single Solid or Surface
 * @file Storage.hh
 * @date 2023-06-01
 * @file C.Gruener
 */

#ifndef SRC_SURFACE_GENERATOR_INCLUDE_STORAGE_HH_
#define SRC_SURFACE_GENERATOR_INCLUDE_STORAGE_HH_

#include <vector>
#include "G4Transform3D.hh"
#include "G4TriangularFacet.hh"
#include "G4Types.hh"

namespace Surface {

/**
 * @brief Struct to describe a single G4Solid
 */
struct SolidDescription {
  enum class Solid { Box, Trd };
  Solid VolumeType;
  std::vector<G4double> VolumeParameter; // volume parameters like width, height
  G4Transform3D Transform;
  std::vector<G4int> OuterSurface; // holds information which Facets are part of the outer surface
};
}
#endif