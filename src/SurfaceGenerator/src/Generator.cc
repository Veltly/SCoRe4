/**
 * @brief
 * @author C.Gruener
 * @date 2023-06-01
 * @file Generator.cc
 */

#include "SurfaceGenerator/include/Generator.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/Assembler.hh"
#include "SurfaceGenerator/include/Calculator.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/FacetStore.hh"

Surface::SurfaceGenerator::SurfaceGenerator(const G4String &name,
                                            const VerboseLevel verboseLvl) noexcept
    : fSolidHandle(nullptr),
      fDescriber(Surface::Describer()),
      fLogger("SurfaceGenerator_" + name, verboseLvl),
      fFacetStore(new FacetStore{name, verboseLvl}),
      fName(name) {
  fLogger.WriteInfo("initialized");
}

void Surface::SurfaceGenerator::GenerateSurface() {
  GenerateDescription();
  Assemble();
  Calculate();
}

void Surface::SurfaceGenerator::Assemble() {
  fLogger.WriteDetailInfo("Calling assemble");
  const auto description = fDescriber.GetSolidDescription();

  Surface::Assembler Assembler(fFacetStore);
  Assembler.SetDescription(description);
  Assembler.Assemble();
  fSolidHandle = Assembler.GetSolid();
  fLogger.WriteDetailInfo("Number of solids used for Assemble: " +
                          std::to_string(fSolidHandle->GetNumberOfSolids()));
}

void Surface::SurfaceGenerator::Calculate() {
  fLogger.WriteDetailInfo("Calling calculate");
  const Calculator calculator{fFacetStore};
  calculator.PrintSurfaceInformation();
}

void Surface::SurfaceGenerator::GenerateDescription() {
  fLogger.WriteDetailInfo("Calling description");
  fDescriber.Generate();
  fLogger.WriteDebugInfo(fDescriber.GetInfoDescription());
}
/**
 * @brief Set global position of FacetStore (translation, no rotation)
 * @param transform
 */
void Surface::SurfaceGenerator::SetSurfaceTransformation(
    const G4ThreeVector &transform) {
  fFacetStore->SetTransformation(transform);
}
