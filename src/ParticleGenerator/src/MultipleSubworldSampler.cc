// Author: C.Gruener
// Date: 24-06-20
// File MultiSubworldSampler

#include "../../Portal/include/MultipleSubworld.hh"
#include "../../Portal/include/PortalStore.hh"
#include "../../Portal/include/SubworldGrid.hh"
#include "../../Service/include/Locator.hh"
#include "../../SurfaceGenerator/include/Calculator.hh"
#include "../../SurfaceGenerator/include/FacetStore.hh"
#include "../include/MultiSubworldSampler.hh"
#include <G4GeneralParticleSource.hh>
#include <G4ThreeVector.hh>

Surface::MultiSubworldSampler::MultiSubworldSampler(const G4String &name,
                                                    const G4String &portalName,
                                                    const G4int verboseLvl)
    : fName(name), fPortalName(portalName), fShift(), fShiftActive(false),
      fSubworldSampler(VSampler<Coord>{"MultiSubworldSampler_" + fName}),
      fSamplerReady(false),
      fLogger({"MultiSubworldSampler_" + fName, verboseLvl}),
      fParticleGenerator(new G4GeneralParticleSource){};

Surface::MultiSubworldSampler::MultiSubworldSampler(
    const G4String &name, const G4String &portalName,
    const G4String &shiftFilename, const G4int verboseLvl)
    : fName(name), fPortalName(portalName), fShift(shiftFilename),
      fShiftActive(true),
      fSubworldSampler(VSampler<Coord>{"MultiSubworldSampler_" + fName}),
      fSamplerReady(false),
      fLogger({"MultiSubworldSampler_" + fName, verboseLvl}),
      fParticleGenerator(new G4GeneralParticleSource){};

void Surface::MultiSubworldSampler::GeneratePrimaryVertex(G4Event *event) {

  if (!fSamplerReady) { // if Sampler not ready
    Surface::PortalStore pStore = Surface::Locator::GetPortalStore();
    G4int portalId = pStore.FindPortalId(fPortalName);
    Surface::MultipleSubworld *subworld =
        static_cast<Surface::MultipleSubworld *>(pStore[portalId]);
    SetSubworld(subworld->GetSubworldGrid());
  }

  fParticleGenerator->GeneratePrimaryVertex(event);
  const G4ThreeVector position = GetRandom();
  event->GetPrimaryVertex()->SetPosition(position.x(), position.y(),
                                         position.z());
}

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

  if (fShiftActive) {
    fShift.DoShift(randomPoint, surfaceNormal);
  }

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
