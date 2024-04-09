//
//
//
//
//	author: C.Gruener
//
//

#ifndef SURFACE_SURFACEGENERATOR
#define SURFACE_SURFACEGENERATOR

#include "../../Service/include/Logger.hh"
#include "Assembler.hh"
#include "Describer.hh"
#include "Storage.hh"
#include <G4VSolid.hh>

namespace Surface {

class SurfaceGenerator {
public:
  SurfaceGenerator() noexcept;
  inline G4VSolid *GetSolid() const { return fSolidhandle; };
  inline std::vector<SurfaceDescription> GetSurface() const {
    return fSurface;
  };
  void GenerateSurface();

private:
  void Assemble();
  void Calculate();
  void GenerateDescription();

  G4VSolid *fSolidhandle{nullptr};
  std::vector<SurfaceDescription> fSurface;
  Describer fDescriber;
  Surface::Logger fLogger{"SurfaceGenerator", 3};
};

} // namespace Surface
#endif
