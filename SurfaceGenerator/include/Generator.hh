//
//
//
//
//	author: C.Gruener
//
//

#include "Assembler.hh"
#include "Calculator.hh"
#include "Describer.hh"
#include "Storage.hh"
#include <G4VSolid.hh>

namespace Surface{

class SurfaceGenerator {
inline  G4VSolid* GetSolid() const;
inline  std::vector<SurfaceDescription> GetSurface() const;
  G4bool GenerateSurface();
  
private:
  G4bool Assemble();
  G4bool Calculate();
  G4bool GenerateDescription();

  G4VSolid* fSolidhandle{nullptr};
  std::vector<SurfaceDescription> fSurface;
};

} // namespace SGenerator
