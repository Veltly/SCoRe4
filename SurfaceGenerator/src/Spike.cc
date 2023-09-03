//
//
//
//
//	author: C.Gruener
//
//
#include "../include/Spike.hh"
#include "../include/Storage.hh"
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>

Surface::Spike::Spike(Spikeform aSpikeform, G4double aWidth_X,
                      G4double aWidth_Y, G4double aHeight, G4int aNLayer)
    : fSpikeform(aSpikeform), fWidth_X(aWidth_X), fWidth_Y(aWidth_Y),
      fHeight(aHeight), fNLayer(aNLayer){};

std::vector<Surface::SolidDescription> Surface::Spike::GetSpikeDescription() {
  if (fSpikeDescription.empty()) {
    GenerateSpike();
  }
  return fSpikeDescription;
}

void Surface::Spike::GenerateSpike() {
  switch (fSpikeform) {
  case Spikeform::Pyramid:
    GeneratePyramide();
    break;
  case Spikeform::Bump:
    GenerateBump();
    break;
  case Spikeform::Peak:
    GeneratePeak();
    break;
  }
}

void Surface::Spike::GeneratePyramide() {
  SolidDescription description;
  description.Volumetype = SolidDescription::Solid::Trd;
  description.Volumeparameter = std::vector<G4double>{
      fWidth_X, fWidthTop_X, fWidth_Y, fWidthTop_Y, fHeight};
  G4ThreeVector MidPoint{0, 0, fHeight};
  description.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
  description.OuterSurface = std::vector<G4int>{
      1, 2, 3, 4,
      5}; // All Facets except bottom (Facet 0) is defined as outer surface
  fSpikeDescription.emplace_back(std::move(description));
}

void Surface::Spike::GenerateBump() {
  // not included yet
}

void Surface::Spike::GeneratePeak() {
  // not included yet
}
