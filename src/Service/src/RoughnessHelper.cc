// Copyright [2024] C.Gruener
// Date: 24-07-15
// File: Helper class to build a roughness G4LogicVolume object

#include "Service/include/RoughnessHelper.hh"

#include "G4MultiUnion.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/Generator.hh"

Surface::RoughnessHelper::RoughnessHelper(const G4int verboseLvl)
    : fLogger("RoughnessHelper", verboseLvl) {}

void Surface::RoughnessHelper::Generate() {
  CheckValues();

  BuildBasis();

  BuildSurface();

  Finalize();
}

// Getter
Surface::Describer &Surface::RoughnessHelper::Describer() {
  return fGenerator.GetDescriber();
}

G4MultiUnion *Surface::RoughnessHelper::Roughness() const { return fRoughness; }

void Surface::RoughnessHelper::CheckValues() {}

void Surface::RoughnessHelper::BuildBasis() {}

void Surface::RoughnessHelper::BuildSurface() {}

void Surface::RoughnessHelper::Finalize() {}
