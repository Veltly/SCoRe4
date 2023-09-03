//
//
//
//
//	author: C.Gruener
//
//

#ifndef SURFACE_ASSEMBLER
#define SURFACE_ASSEMBLER

#include "Storage.hh"
#include <G4Box.hh>
#include <G4Transform3D.hh>
#include <G4Trd.hh>
#include <G4VSolid.hh>
#include <vector>

using Description = std::vector<Surface::SolidDescription>;

namespace Surface {

class Assembler {
public:
  void Assemble();
  G4VSolid *GetSolid();
  void SetDescription(Description &aDescription);

private:
  void AddToFacetStore(const G4VSolid&, const G4Transform3D&, const std::vector<G4int>&);
  void AddToFacetStore(const SolidDescription&);
  G4VSolid *GetSingleSolid(const SolidDescription &);
  G4VSolid *GetBox(const SolidDescription &);
  G4VSolid *GetTrd(const SolidDescription &);
  G4String GenerateSolidName(const SolidDescription&);
  G4VSolid *fSolid;
  Description fDescription;

};

} // namespace Surface
#endif
