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
  void GenerateSurface();
  
private:
  void Assemble();
  void Calculate();
  void GenerateDescription();

  G4VSolid* fSolidhandle{nullptr};
  std::vector<SurfaceDescription> fSurface;
  Describer fDescriber;
};

} // namespace SGenerator
