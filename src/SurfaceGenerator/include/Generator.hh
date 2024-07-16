// Copyright [2024] C.Gruener
// 23-06-01
// File:

#ifndef SRC_SURFACEGENERATOR_INCLUDE_GENERATOR_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_GENERATOR_HH_
#include <vector>

#include "G4VSolid.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/Assembler.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
#include "SurfaceGenerator/include/Storage.hh"

namespace Surface {
///
/// Controls generation a solid volume which represents a patch of rough
/// surface. A handle to this volume can be received to place it as a physical
/// volume in simulation. Furthermore a FacetStore which references to the outer
/// surface of the volume will be generated.
class SurfaceGenerator {
 public:
  SurfaceGenerator(const G4String &name) noexcept;
  SurfaceGenerator(const G4String &name, const G4int verboseLvl) noexcept;
  ///
  /// Returns handle to solid
  inline G4VSolid *GetSolid() const { return fSolidhandle; }

  ///
  /// Generates solid
  void GenerateSurface();

  inline Describer &GetDescriber() { return fDescriber; }
  inline FacetStore *GetFacetStore() { return fFacetStore; }
  void SetSurfaceTransformation(const G4ThreeVector &transform);

 private:
  inline std::vector<SurfaceDescription> GetSurface() const { return fSurface; }
  ///
  /// Calls and executes Assembler to generate a Solid based on description
  void Assemble();
  ///
  /// Calls calculator to get SurfaceParameters
  void Calculate();
  ///
  /// Calls Describer to generate description of surface
  void GenerateDescription();

  G4VSolid *fSolidhandle;                    ///< Pointer to final solid
  std::vector<SurfaceDescription> fSurface;  ///< stores description of surface
  Describer fDescriber;                      ///< describes surface
  Logger fLogger;
  FacetStore *fFacetStore;
  G4String fName;
};

}  // namespace Surface
#endif  // SRC_SURFACEGENERATOR_INCLUDE_GENERATOR_HH_
