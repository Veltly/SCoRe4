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

G4bool SURFACE::SurfaceGenerator::GenerateSurface() {
  GenerateDescription();
  Assemble();
  Calculate();
  return true;
}

