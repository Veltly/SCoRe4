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
#include <G4Transform3D.hh>
#include <G4VSolid.hh>

namespace Surface {
///
/// Controls generation a solid volume which represents a patch of rough
/// surface. A handle to this volume can be received to place it as a physical
/// volume in simulation. Furthermore a FacetStore which references to the outer
/// surface of the volume will be generated.
class SurfaceGenerator {
public:
  SurfaceGenerator() noexcept;
  ///
  /// Returns handle to solid
  inline G4VSolid *GetSolid() const { return fSolidhandle; };

  ///
  /// Generates solid
  void GenerateSurface();

  inline Describer &GetDescriber() { return fDescriber; };
  void SetSurfaceTransformation(G4ThreeVector &transform);

private:
  inline std::vector<SurfaceDescription> GetSurface() const {
    return fSurface;
  };
  ///
  /// Calls and executes Assembler to generate a Solid based on description
  void Assemble();
  ///
  /// Calls calculator to get SurfaceParameters
  void Calculate();
  ///
  /// Calls Describer to generate description of surface
  void GenerateDescription();

  G4VSolid *fSolidhandle;                   ///< Pointer to final solid
  std::vector<SurfaceDescription> fSurface; ///< stores description of surface
  Describer fDescriber;                     ///< describes surface
  Surface::Logger fLogger;
};

} // namespace Surface
#endif
