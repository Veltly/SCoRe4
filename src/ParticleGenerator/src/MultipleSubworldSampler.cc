// Copyright [2024] C.Gruener
// Date: 24-06-20
// File MultiSubworldSampler

#include <cstdlib>

#include "G4GeneralParticleSource.hh"
#include "G4ThreeVector.hh"
#include "ParticleGenerator/include/MultiSubworldSampler.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/PortalStore.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Service/include/Locator.hh"
#include "SurfaceGenerator/include/Calculator.hh"
#include "SurfaceGenerator/include/FacetStore.hh"

Surface::MultiSubworldSampler::MultiSubworldSampler(const G4String &name,
                                                    const G4String &portalName,
                                                    const G4int verboseLvl)
    : fName(name),
      fPortalName(portalName),
      fShift(),
      fShiftActive(false),
      fSubworldSampler(VSampler<Coord>{"MultiSubworldSampler_" + fName}),
      fSamplerReady(false),
      fLogger({"MultiSubworldSampler_" + fName, verboseLvl}),
      fParticleGenerator(new G4GeneralParticleSource) {}

Surface::MultiSubworldSampler::MultiSubworldSampler(
    const G4String &name, const G4String &portalName,
    const G4String &shiftFilename, const G4int verboseLvl)
    : fName(name),
      fPortalName(portalName),
      fShift(shiftFilename),
      fShiftActive(true),
      fSubworldSampler(VSampler<Coord>{"MultiSubworldSampler_" + fName}),
      fSamplerReady(false),
      fLogger({"MultiSubworldSampler_" + fName, verboseLvl}),
      fParticleGenerator(new G4GeneralParticleSource) {}

void Surface::MultiSubworldSampler::GeneratePrimaryVertex(G4Event *event) {
  if (!fSamplerReady) {  // if Sampler not ready
    fLogger.WriteInfo("Sampler not ready -> now preparing ...");

    Surface::PortalStore pStore = Surface::Locator::GetPortalStore();
    const G4int portalId = pStore.FindPortalId(fPortalName);
    if (portalId < 0) {
      fLogger.WriteError("Error: No portal with name \"" + fPortalName +
                         "\" found!!");
      fLogger.WriteError("Possible portals in Store are:");
      for (auto &portal : pStore) {
        fLogger.WriteError(portal->GetName());
      }

      exit(EXIT_FAILURE);
    }

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

  const Coord randomCoord = fSubworldSampler.GetRandom();

  fSubworld->SetCurrentX(randomCoord.x);
  fSubworld->SetCurrentY(randomCoord.y);

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

  fLogger.WriteDebugInfo(
      "Selected Subworld X: " + std::to_string(randomCoord.x) +
      " Y: " + std::to_string(randomCoord.y));
  return randomPoint;
}

void Surface::MultiSubworldSampler::PrepareSampler() {
  const std::set<MultipleSubworld *> unique = fSubworld->GetUniqueSubworlds();
  std::map<MultipleSubworld *, G4double> surfaceArea;
  for (auto *subworld : unique) {
    fLogger.WriteDetailInfo("From Subworld: " + subworld->GetName());
    FacetStore *facetStore = subworld->GetFacetStore();
    fLogger.WriteDetailInfo("get FacetStore: " + facetStore->GetStoreName());
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
  fLogger.WriteInfo("MultiSubworldSampler is ready.");
}

void Surface::MultiSubworldSampler::SetSubworld(
    SubworldGrid<MultipleSubworld> *subworldGrid) {
  fSubworld = subworldGrid;
  fLogger.WriteInfo("SubworldGrid set");
}
