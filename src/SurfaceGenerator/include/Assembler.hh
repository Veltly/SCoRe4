//
//
//
//
//	author: C.Gruener
//
//

#ifndef SURFACE_ASSEMBLER
#define SURFACE_ASSEMBLER

#include "../../Service/include/Logger.hh"
#include "FacetStore.hh"
#include "Storage.hh"
#include <G4Box.hh>
#include <G4Transform3D.hh>
#include <G4Trd.hh>
#include <G4VSolid.hh>
#include <vector>

using Description = std::vector<Surface::SolidDescription>;

namespace Surface {
///
/// Assembles a solid from basic solids based on Description.
/// All used basic solids have to be implemented in the Assembler class.
///
class Assembler {
public:
  Assembler(FacetStore *store);
  ///
  /// calls the assembler to assemble and add all selected facets to the
  /// FacetStore.
  void Assemble();
  ///
  /// returns handle to assembled solid.
  inline G4VSolid *GetSolid() const { return fSolid; };
  ///
  /// sets description for assembly.
  inline void SetDescription(Description &aDescription) {
    fDescription = aDescription;
  };

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
  G4VSolid *fSolid;         ///< Solid handle;
  Description fDescription; ///< handle of Description
  Surface::Logger fLogger;
  FacetStore *fFacetStore;
};

} // namespace Surface
#endif
