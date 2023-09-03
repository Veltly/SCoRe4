//
//
//
//
//	author: C.Gruener
//
//
#include "../include/Assembler.hh"
#include "../include/Calculator.hh"
#include "../include/Describer.hh"
#include "../include/Generator.hh"

void Surface::SurfaceGenerator::GenerateSurface() {
  GenerateDescription();
  Assemble();
  Calculate();
}

void Surface::SurfaceGenerator::Assemble(){
  auto description = fDescriber.GetSolidDescription();
  Surface::Assembler Assembler;
  Assembler.SetDescription(description);
  Assembler.Assemble();
  fSolidhandle = Assembler.GetSolid();
}

void Surface::SurfaceGenerator::Calculate(){


}

void Surface::SurfaceGenerator::GenerateDescription(){
  fDescriber.Generate();
}
