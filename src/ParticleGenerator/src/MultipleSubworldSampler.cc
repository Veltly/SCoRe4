// Author: C.Gruener
// Date: 24-06-20
// File MultiSubworldSampler

#include "../../Portal/include/MultipleSubworld.hh"
#include "../include/MultiSubworldSampler.hh"
#include "SurfaceGenerator/include/Calculator.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
#include <G4ThreeVector.hh>

Surface::MultiSubworldSampler::MultiSubworldSampler(G4String shiftFilename)
    : fShift({shiftFilename}),
      fSubworldSampler(VSampler<Coord>{"MultiSubworldSampler"}),
      fSamplerReady(false), fLogger({"MultiSubworldSampler", 3}){};

G4ThreeVector Surface::MultiSubworldSampler::GetRandom() {
  if (!fSamplerReady) {
    PrepareSampler();
  }
  const Coord randCoord = fSubworldSampler.GetRandom();
  fSubworld->SetCurrentX(randCoord.x);
  fSubworld->SetCurrentY(randCoord.y);
  MultipleSubworld *subworld = fSubworld->GetSubworld();
  FacetStore *facetStore = subworld->GetFacetStore();
  if (!facetStore->GetIsStoreClosed()) {
    facetStore->CloseFacetStore();
  }
  G4ThreeVector surfaceNormal;
  G4ThreeVector randomPoint = facetStore->GetRandomPoint(surfaceNormal);
  fShift.DoShift(randomPoint, surfaceNormal);
  // fShift.DoShiftByValue(0.1 * CLHEP::mm, randomPoint, surfaceNormal);
  fLogger.WriteDebugInfo("Selected Subworld X: " + std::to_string(randCoord.x) +
                         " Y: " + std::to_string(randCoord.y));
  return randomPoint;
};

void Surface::MultiSubworldSampler::PrepareSampler() {
  const std::set<MultipleSubworld *> unique = fSubworld->GetUniqueSubworlds();
  std::map<MultipleSubworld *, G4double> surfaceArea;
  for (auto *subworld : unique) {
    auto *facetStore = subworld->GetFacetStore();
    Calculator calc(facetStore);
    surfaceArea[subworld] = calc.GetArea();
  }
  const G4int maxX = fSubworld->MaxX();
  const G4int maxY = fSubworld->MaxY();
  for (G4int x = 0; x < maxX; ++x) {
    for (G4int y = 0; y < maxY; ++y) {
      auto *subworld = fSubworld->GetSubworld(x, y);
      fSubworldSampler.AppendValue(Coord{x, y});
      fSubworldSampler.AppendProbability(surfaceArea[subworld]);
    }
  }
  fSamplerReady = true;
  fLogger.WriteDebugInfo("Prepared MultiSubworldSampler");
}

void Surface::MultiSubworldSampler::SetSubworld(
    SubworldGrid<MultipleSubworld> *subworldGrid) {
  fSubworld = subworldGrid;
  fLogger.WriteDebugInfo("SubworldGrid set");
}
