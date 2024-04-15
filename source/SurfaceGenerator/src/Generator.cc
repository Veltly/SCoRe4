//
//
//
//
//	author: C.Gruener
//
//
#include "../include/Generator.hh"
#include "../../Service/include/Locator.hh"
#include "../include/Assembler.hh"
#include "../include/Calculator.hh"
#include "../include/Describer.hh"
#include <G4MultiUnion.hh>
Surface::SurfaceGenerator::SurfaceGenerator() noexcept
    : fSolidhandle(nullptr), fLogger({"SurfaceGenerator", 3}) {
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
  Surface::Assembler Assembler;
  Assembler.SetDescription(description);
  Assembler.Assemble();
  fSolidhandle = Assembler.GetSolid();
  G4MultiUnion *test = static_cast<G4MultiUnion *>(Assembler.GetSolid());
  fLogger.WriteDebugInfo(test->GetNumberOfSolids());
}

void Surface::SurfaceGenerator::Calculate() {
  fLogger.WriteDebugInfo("calling calculate");
  Calculator calculator{Locator::GetFacetStore()};
  calculator.PrintSurfaceInformation();
}

void Surface::SurfaceGenerator::GenerateDescription() {
  fLogger.WriteDebugInfo("calling descrition");
  fDescriber.Generate();
  fLogger.WriteDebugInfo(fDescriber.GetInfoDescription());
}
