/**
 * @brief Controls surface generation
 * @author C.Gruener
 * @date 2023-06-01
 * @file Generator.hh
 */
#ifndef SRC_SURFACE_GENERATOR_INCLUDE_GENERATOR_HH_
#define SRC_SURFACE_GENERATOR_INCLUDE_GENERATOR_HH_

#include <vector>
#include "G4VSolid.hh"
#include "G4MultiUnion.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/Assembler.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
#include "SurfaceGenerator/include/Storage.hh"

namespace Surface {
/**
 * @brief Controls surface generation
 * @details Controls generation of a solid volume which represents a patch of rough
 * surface. A handle to this volume can be received to place it as a physical
 * volume in the simulation. Furthermore, a FacetStore with handles to the outer
 * surface of the volume will be generated.
 */
class SurfaceGenerator {
 public:
  //explicit SurfaceGenerator(const G4String &name) noexcept;
  explicit SurfaceGenerator(const G4String &name, VerboseLevel verboseLvl = VerboseLevel::Default) noexcept;
  /**
   * @brief Returns handle to generated solid
   * @return pointer to generated solid
   */
  inline G4MultiUnion *GetSolid() const { return fSolidHandle; }

  /**
   * @brief Generate solid representing the surface
   */
  void GenerateSurface();

  /**
   * @brief Reference to describer instance. Is used to define the patch of rough surface using parameters
   * @return Reference to describer instance.
   */
  inline Describer &GetDescriber() { return fDescriber; }

  /**
   * @brief Get a handle to the FacetStore which stores the outer surface of the generated volume.
   * @return Handle to FacetStore
   */
  inline FacetStore *GetFacetStore() { return fFacetStore; }
  /**
   * @brief Add transformation to FacetStore to "place" it in the simulation domain
   * @param transform
   */
  void SetSurfaceTransformation(const G4ThreeVector &transform);

 private:

  /**
   * @brief Calls and executes Assembler to generate a Solid based on description
   */
  void Assemble();

  /**
   * @brief Calls calculator to get SurfaceParameters
   */
  void Calculate();

  /**
   * @brief Calls Describer instance to generate description of surface
   */
  void GenerateDescription();

  G4MultiUnion *fSolidHandle;                /// Pointer to final solid
  Describer fDescriber;                      /// describes surface
  Logger fLogger;
  FacetStore *fFacetStore;                   /// Handle to FacetStore
  G4String fName;
};
}
#endif
