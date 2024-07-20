// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:
//
#include "SurfaceGenerator/include/Generator.hh"

#include "G4MultiUnion.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/Assembler.hh"
#include "SurfaceGenerator/include/Calculator.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/FacetStore.hh"

Surface::SurfaceGenerator::SurfaceGenerator(const G4String &name) noexcept
    : fSolidhandle(nullptr),
      fDescriber(Surface::Describer()),
      fLogger("SurfaceGenerator_" + name),
      fFacetStore(new FacetStore{name}),
      fName(name) {
  fLogger.WriteInfo("initialized");
}

Surface::SurfaceGenerator::SurfaceGenerator(const G4String &name,
                                            const G4int verboseLvl) noexcept
    : fSolidhandle(nullptr),
      fDescriber(Surface::Describer()),
      fLogger("SurfaceGenerator_" + name, verboseLvl),
      fFacetStore(new FacetStore{name}),
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

  fSolidhandle = Assembler.GetSolid();
  const G4MultiUnion *test = static_cast<G4MultiUnion *>(fSolidhandle);
  fLogger.WriteDetailInfo("Number of solids used for Assemble: " +
                          std::to_string(test->GetNumberOfSolids()));
}

void Surface::SurfaceGenerator::Calculate() {
  fLogger.WriteDetailInfo("Calling calculate");
  const Calculator calculator{fFacetStore};
  calculator.PrintSurfaceInformation();
}

void Surface::SurfaceGenerator::GenerateDescription() {
  fLogger.WriteDetailInfo("Calling descrition");
  fDescriber.Generate();
  fLogger.WriteDebugInfo(fDescriber.GetInfoDescription());
}

void Surface::SurfaceGenerator::SetSurfaceTransformation(
    const G4ThreeVector &transform) {
  fFacetStore->SetTransformation(transform);
}
