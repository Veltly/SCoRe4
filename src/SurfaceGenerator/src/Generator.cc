//
//
//
//
//	author: C.Gruener
//
//
#include "../include/Generator.hh"
#include "../include/Assembler.hh"
#include "../include/Calculator.hh"
#include "../include/Describer.hh"
#include "../include/FacetStore.hh"
#include <G4MultiUnion.hh>
#include <G4Transform3D.hh>
Surface::SurfaceGenerator::SurfaceGenerator(G4String name) noexcept
    : fSolidhandle(nullptr), fDescriber(Surface::Describer()),
      fLogger({"SurfaceGenerator", 3}), fFacetStore(new FacetStore{name}),
      fName(name) {
  fLogger.WriteInfo("initialized");
}

void Surface::SurfaceGenerator::GenerateSurface() {
  GenerateDescription();
  Assemble();
  Calculate();
}

void Surface::SurfaceGenerator::Assemble() {
  fLogger.WriteDebugInfo("calling assemble");
  auto description = fDescriber.GetSolidDescription();
  Surface::Assembler Assembler(fFacetStore);
  Assembler.SetDescription(description);
  Assembler.Assemble();
  fSolidhandle = Assembler.GetSolid();
  G4MultiUnion *test = static_cast<G4MultiUnion *>(fSolidhandle);
  fLogger.WriteDebugInfo(std::to_string(test->GetNumberOfSolids()));
}

void Surface::SurfaceGenerator::Calculate() {
  fLogger.WriteDebugInfo("calling calculate");
  Calculator calculator{fFacetStore};
  calculator.PrintSurfaceInformation();
}

void Surface::SurfaceGenerator::GenerateDescription() {
  fLogger.WriteDebugInfo("calling descrition");
  fDescriber.Generate();
  fLogger.WriteDebugInfo(fDescriber.GetInfoDescription());
}

void Surface::SurfaceGenerator::SetSurfaceTransformation(
    G4ThreeVector &transform) {
  fFacetStore->SetTransformation(transform);
}
