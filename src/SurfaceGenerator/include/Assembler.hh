/**
 * @brief Declares the Assembler class to generate a G4Solid based on a SolidDescription
 * @file Assembler.hh
 * @date 2023-06-01
 * @author C.Gruener
 */
#ifndef SRC_SURFACE_GENERATOR_INCLUDE_ASSEMBLER_HH_
#define SRC_SURFACE_GENERATOR_INCLUDE_ASSEMBLER_HH_

#include <vector>
#include "G4VSolid.hh"
#include "G4MultiUnion.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
#include "SurfaceGenerator/include/Storage.hh"

using Description = std::vector<Surface::SolidDescription>;

namespace Surface {
/**
 * @class Assembler
 * @brief Assembles a G4Solid from basic solids based on @ref Surface::SolidDescription.
 * All used basic solids have to be implemented in the Assembler class.
 */
class Assembler {
 public:
  /**
   * @brief Constructor for assembler class
   * @param store handle to FacetStore that should be used.
   */
  explicit Assembler(FacetStore *store);

  /**
   * @brief Calls the assembler to assemble solid and add all selected facets to the FacetStore.
   */
  void Assemble();

  /**
   * @brief Get handle G4Solid after assembling it.
   * @return Returns pointer to generated solid.
   */
  G4MultiUnion *GetSolid() const;

  /**
   * @brief Pass the description of the solid to assembler instance.
   * @param aDescription
   */
  inline void SetDescription(const Description &aDescription) {
    fDescription = aDescription;
  }

 private:
  /**
   * @brief Adds selected facets of solid to FacetStore based on description.
   * @param SolidDescription Description of solid
   */
  void AddToFacetStore(const SolidDescription &);
  /**
   * @brief Get solid based on description. All possible solids must be selectable in this function.
   * @return
   */
  static G4VSolid *GetSingleSolid(const SolidDescription &);
  /**
   * @brief Get solid G4Box based on description.
   * @return pointer to G4Box
   */
  static G4VSolid *GetBox(const SolidDescription &);
  /**
   * @brief Get solid G4Trd based on description.
   * @return pointer to G4Trd
   */
  static G4VSolid *GetTrd(const SolidDescription &);

  /**
   * @brief Generates a name for solid based on description.
   * Name should be unique based on generation algorithm, but this is not
   * checked.
   * @return name for solid
   */
  static G4String GenerateSolidName(const SolidDescription &);

  G4MultiUnion *fSolid{nullptr}; // solid handle;
  Description fDescription;  // description
  Surface::Logger fLogger;
  FacetStore *fFacetStore;
};

}
#endif
