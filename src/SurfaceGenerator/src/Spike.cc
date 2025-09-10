/**
 * @brief
 * @file Spike.cc
 * @date 2023-06-01
 * @author C.Gruener
 */

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

/**
 * @brief General function to generate the spike description
 */
void Surface::Spike::GenerateSpike() {
  switch (fSpikeform) {
    case Spikeform::Pyramid:
      GeneratePyramid();
      return;
    case Spikeform::Bump:
      GenerateBump();
      return;
    case Spikeform::Peak:
      GeneratePeak();
      return;
    default:
      EXIT_FAILURE;
  }
}

/**
 * @brief generates a pyramid like spike with one layer
 */
void Surface::Spike::GeneratePyramid() {
  SolidDescription description;
  description.VolumeType = SolidDescription::Solid::Trd;
  description.VolumeParameter = std::vector<G4double>{
      fWidth_X, fWidthTop_X, fWidth_Y, fWidthTop_Y, fHeight};
  const G4ThreeVector MidPoint{0, 0, fHeight};
  description.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
  description.OuterSurface = std::vector<G4int>{2, 3, 4, 5, 6};
  // All Facets except bottom (Facet 1) are defined as outer surface
  fSpikeDescription.emplace_back(std::move(description));
}

/**
 * @brief generates a bump like spike with multiple layers. Spikes are generated from bottom to top.
 */
void Surface::Spike::GenerateBump() {
  const G4double deltaHeight = fHeight / fNLayer;
  G4double base_X = fWidth_X;
  G4double base_Y = fWidth_Y;
  for (G4int i = 1; i < fNLayer; ++i) {
    const G4double currentTop = i * deltaHeight;
    const G4double top_X = FunctionBump(currentTop, fWidth_X);
    const G4double top_Y = FunctionBump(currentTop, fWidth_Y);
    SolidDescription LayerDescription;
    LayerDescription.VolumeType = SolidDescription::Solid::Trd;
    LayerDescription.VolumeParameter =
        std::vector<G4double>{base_X, top_X, base_Y, top_Y, deltaHeight / 2.};
    G4ThreeVector MidPoint{0, 0, currentTop - deltaHeight / 2.};
    LayerDescription.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
    LayerDescription.OuterSurface = std::vector<G4int>{2, 3, 4, 5};
    // All Facets except bottom, top (Facet 1, 6) are defined as outer surface
    base_X = top_X;
    base_Y = top_Y;
    fSpikeDescription.emplace_back(std::move(LayerDescription));
  }
  // Top
  SolidDescription LastLayer;

  LastLayer.VolumeType = SolidDescription::Solid::Trd;
  LastLayer.VolumeParameter = std::vector<G4double>{
      base_X, fWidthTop_X, base_Y, fWidthTop_Y, deltaHeight / 2.};
  G4ThreeVector MidPoint{0, 0, fHeight - deltaHeight / 2.};
  LastLayer.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
  LastLayer.OuterSurface = std::vector<G4int>{2, 3, 4, 5, 6};
  // All Facets except bottom (Facet 1) are defined as outer surface
  fSpikeDescription.emplace_back(std::move(LastLayer));
}

/**
 * @brief generates a peak like spike with multiple layers. Spikes are generated from bottom to top.
 */
void Surface::Spike::GeneratePeak() {
  const G4double deltaHeight = fHeight / fNLayer;
  G4double base_X = fWidth_X;
  G4double base_Y = fWidth_Y;
  for (G4int i = 1; i < fNLayer; ++i) {
    const G4double currentTop = i * deltaHeight;
    const G4double top_X = FunctionPeak(currentTop, fWidth_X);
    const G4double top_Y = FunctionPeak(currentTop, fWidth_Y);
    SolidDescription LayerDescription;
    LayerDescription.VolumeType = SolidDescription::Solid::Trd;
    LayerDescription.VolumeParameter =
        std::vector<G4double>{base_X, top_X, base_Y, top_Y, deltaHeight / 2.};
    const G4ThreeVector MidPoint{0, 0, currentTop - deltaHeight / 2.};
    LayerDescription.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
    LayerDescription.OuterSurface = std::vector<G4int>{2, 3, 4, 5};
    // All Facets except bottom, top (Facet 1, 6) are defined as outer surface
    base_X = top_X;
    base_Y = top_Y;
    fSpikeDescription.emplace_back(std::move(LayerDescription));
  }
  // Top
  SolidDescription LastLayer;

  LastLayer.VolumeType = SolidDescription::Solid::Trd;
  LastLayer.VolumeParameter = std::vector<G4double>{
      base_X, fWidthTop_X, base_Y, fWidthTop_Y, deltaHeight / 2.};
  const G4ThreeVector MidPoint{0, 0, fHeight - deltaHeight / 2.};
  LastLayer.Transform = G4Transform3D(G4RotationMatrix(), MidPoint);
  LastLayer.OuterSurface = std::vector<G4int>{2, 3, 4, 5, 6};
  // All Facets except bottom (Facet 1) are defined as outer surface
  fSpikeDescription.emplace_back(std::move(LastLayer));
}

/**
 * @brief defines the outer shape of the spike 'bump'
 * @param aNextHeight height of cross-section of spike to calculate the width
 * @param aBaseSide bottom segment width
 * @return top segment width
 */
G4double Surface::Spike::FunctionBump(const G4double aNextHeight,
                                      const G4double aBaseSide) const {
  return std::abs(aBaseSide * pow(aNextHeight / fHeight, 2) - aBaseSide);
}

/**
 * @brief defines the outer shape of the spike 'peak'
 * @param aNextHeight height of cross-section of spike to calculate the width
 * @param aBaseSide bottom segment width
 * @return top segment width
 */
G4double Surface::Spike::FunctionPeak(const G4double aNextHeight,
                                      const G4double aBaseSide) const {
  const G4double paraA = aNextHeight / fHeight;
  const G4double paraB{5.}; // parameter for shape function
  const G4int exponent{1}; // parameter for shape function
  return aBaseSide * 1. / std::pow(paraB * paraA + 1, exponent);
}
