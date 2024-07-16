// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#include "SurfaceGenerator/include/Spike.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "SurfaceGenerator/include/Storage.hh"

Surface::Spike::Spike(Spikeform aSpikeform, G4double aWidth_X,
                      G4double aWidth_Y, G4double aHeight, G4int aNLayer)
    : fSpikeform(aSpikeform),
      fWidth_X(aWidth_X),
      fWidth_Y(aWidth_Y),
      fHeight(aHeight),
      fNLayer(aNLayer) {}

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
  const G4ThreeVector MidPoint{0, 0, fHeight};
  description.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
  description.OuterSurface = std::vector<G4int>{
      2, 3, 4, 5,
      6};  // All Facets except bottom (Facet 1) is defined as outer surface
  fSpikeDescription.emplace_back(std::move(description));
}

void Surface::Spike::GenerateBump() {
  const G4double deltaHeight = fHeight / fNLayer;
  G4double base_X = fWidth_X;
  G4double base_Y = fWidth_Y;
  for (G4int i = 1; i < fNLayer; ++i) {
    const G4double currentTop = i * deltaHeight;
    const G4double top_X = FunctionBump(currentTop, fWidth_X);
    const G4double top_Y = FunctionBump(currentTop, fWidth_Y);
    SolidDescription LayerDescription;
    LayerDescription.Volumetype = SolidDescription::Solid::Trd;
    LayerDescription.Volumeparameter =
        std::vector<G4double>{base_X, top_X, base_Y, top_Y, deltaHeight / 2.};
    G4ThreeVector MidPoint{0, 0, currentTop - deltaHeight / 2.};
    LayerDescription.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
    LayerDescription.OuterSurface = std::vector<G4int>{2, 3, 4, 5};
    base_X = top_X;
    base_Y = top_Y;
    fSpikeDescription.emplace_back(std::move(LayerDescription));
  }
  // Top
  SolidDescription LastLayer;

  LastLayer.Volumetype = SolidDescription::Solid::Trd;
  LastLayer.Volumeparameter = std::vector<G4double>{
      base_X, fWidthTop_X, base_Y, fWidthTop_Y, deltaHeight / 2.};
  G4ThreeVector MidPoint{0, 0, fHeight - deltaHeight / 2.};
  LastLayer.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
  LastLayer.OuterSurface = std::vector<G4int>{2, 3, 4, 5, 6};
  fSpikeDescription.emplace_back(std::move(LastLayer));
}

void Surface::Spike::GeneratePeak() {
  const G4double deltaHeight = fHeight / fNLayer;
  G4double base_X = fWidth_X;
  G4double base_Y = fWidth_Y;
  for (G4int i = 1; i < fNLayer; ++i) {
    const G4double currentTop = i * deltaHeight;
    const G4double top_X = FunctionPeak(currentTop, fWidth_X);
    const G4double top_Y = FunctionPeak(currentTop, fWidth_Y);
    SolidDescription LayerDescription;
    LayerDescription.Volumetype = SolidDescription::Solid::Trd;
    LayerDescription.Volumeparameter =
        std::vector<G4double>{base_X, top_X, base_Y, top_Y, deltaHeight / 2.};
    const G4ThreeVector MidPoint{0, 0, currentTop - deltaHeight / 2.};
    LayerDescription.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
    LayerDescription.OuterSurface = std::vector<G4int>{2, 3, 4, 5};
    base_X = top_X;
    base_Y = top_Y;
    fSpikeDescription.emplace_back(std::move(LayerDescription));
  }
  // Top
  SolidDescription LastLayer;

  LastLayer.Volumetype = SolidDescription::Solid::Trd;
  LastLayer.Volumeparameter = std::vector<G4double>{
      base_X, fWidthTop_X, base_Y, fWidthTop_Y, deltaHeight / 2.};
  const G4ThreeVector MidPoint{0, 0, fHeight - deltaHeight / 2.};
  LastLayer.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
  LastLayer.OuterSurface = std::vector<G4int>{2, 3, 4, 5, 6};
  fSpikeDescription.emplace_back(std::move(LastLayer));
}

G4double Surface::Spike::FunctionBump(const G4double aNextHeight,
                                      const G4double aBaseside) {
  return std::abs(aBaseside * pow(aNextHeight / fHeight, 2) - aBaseside);
}

G4double Surface::Spike::FunctionPeak(const G4double aNextHeight,
                                      const G4double aBaseside) {
  const G4double paraA = aNextHeight / fHeight;
  const G4double paraB{5.};
  const G4int potenz{1};
  return aBaseside * 1. / std::pow(paraB * paraA + 1, potenz);
}
