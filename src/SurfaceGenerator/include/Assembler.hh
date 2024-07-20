// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#ifndef SRC_SURFACEGENERATOR_INCLUDE_ASSEMBLER_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_ASSEMBLER_HH_

#include <vector>

#include "G4VSolid.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
#include "SurfaceGenerator/include/Storage.hh"

using Description = std::vector<Surface::SolidDescription>;

namespace Surface {
///
/// Assembles a solid from basic solids based on Description.
/// All used basic solids have to be implemented in the Assembler class.
///
class Assembler {
 public:
  explicit Assembler(FacetStore *store);
  ///
  /// calls the assembler to assemble and add all selected facets to the
  /// FacetStore.
  void Assemble();
  ///
  /// returns handle to assembled solid.
  inline G4VSolid *GetSolid() const { return fSolid; }
  ///
  /// sets description for assembly.
  inline void SetDescription(const Description &aDescription) {
    fDescription = aDescription;
  }

 private:
  ///
  /// Adds selected Facets of solid to FacetStore based on Description.
  void AddToFacetStore(const SolidDescription &);
  ///
  /// Get solid based on description. All possible solids must be selectable in
  /// this function.
  G4VSolid *GetSingleSolid(const SolidDescription &);
  G4VSolid *GetBox(const SolidDescription &);
  G4VSolid *GetTrd(const SolidDescription &);
  ///
  /// Generate a name for solid based on description.
  /// Name should be unique based on generation algorithm, but this is not
  /// checked.
  G4String GenerateSolidName(const SolidDescription &);
  G4VSolid *fSolid;          ///< Solid handle;
  Description fDescription;  ///< handle of Description
  Surface::Logger fLogger;
  FacetStore *fFacetStore;
};

}  // namespace Surface
#endif  // SRC_SURFACEGENERATOR_INCLUDE_ASSEMBLER_HH_
