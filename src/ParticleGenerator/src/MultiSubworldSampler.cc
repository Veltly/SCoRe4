/**
 * @brief Implementation of class MultiSubworldSampler
 * @author C.Gruener
 * @date 2024-06-20
 * @file MultiSubworldSampler.cc
 */

#include <utility>

#include "G4GeneralParticleSource.hh"
#include "G4ThreeVector.hh"
#include "ParticleGenerator/include/MultiSubworldSampler.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/PortalStore.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Service/include/Locator.hh"
#include "SurfaceGenerator/include/Calculator.hh"
#include "SurfaceGenerator/include/FacetStore.hh"

std::ostream &Surface::operator<<(std::ostream &os, const Coord &coord) {
  os << coord.x << ":" << coord.y;
  return os;
}

Surface::MultiSubworldSampler::MultiSubworldSampler(G4String name,
                                                    G4String portalName,
                                                    const VerboseLevel verboseLvl)
    : fName{std::move(name)},
      fPortalName{std::move(portalName)},
      fShift{verboseLvl},
      fShiftActive(false),
      fSubworldSampler(VSampler<Coord>{"MultiSubworldSampler_" + fName}),
      fSamplerReady(false),
      fLogger("MultiSubworldSampler_" + fName, verboseLvl),
      fParticleGenerator(new G4GeneralParticleSource) {
}

Surface::MultiSubworldSampler::MultiSubworldSampler(
    G4String name, G4String portalName,
    const G4String &shiftFilename, const VerboseLevel verboseLvl)
    : fName(std::move(name)),
      fPortalName(std::move(portalName)),
      fShift(shiftFilename, verboseLvl),
      fShiftActive(true),
      fSubworldSampler(VSampler<Coord>{"MultiSubworldSampler_" + fName}),
      fSamplerReady(false),
      fLogger("MultiSubworldSampler_" + fName, verboseLvl),
      fParticleGenerator(new G4GeneralParticleSource) {
}

Surface::MultiSubworldSampler::~MultiSubworldSampler() {

}

void Surface::MultiSubworldSampler::GeneratePrimaryVertex(G4Event *event) {
  if (!fSamplerReady) {  // if Sampler not ready
    fLogger.WriteInfo("Sampler not ready -> now preparing ...");

    Surface::PortalStore pStore = Surface::Locator::GetPortalStore();
    const G4int portalId = pStore.FindPortalId(fPortalName);
    if (portalId < 0) {
      fLogger.WriteError("Error: No portal with name \"" + fPortalName +
                         "\" found!!");
      G4String possible_names = "Possible names: ";
      fLogger.WriteError("Possible portals in Store are:");
      for (auto &portal : pStore) {
        possible_names = possible_names + ", " + portal->GetName();
        fLogger.WriteError(portal->GetName());
      }
      G4String error_msg = "Error: No portal with name " + fPortalName + " found!\n" + possible_names;
      G4Exception("MultiSubworldSampler::GeneratePrimaryVertex()",
                  "",
                  FatalException,
                  error_msg);
    }

    auto *subworld =
        dynamic_cast<Surface::MultipleSubworld *>(pStore[portalId]);
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
  fLogger.WriteDetailInfo([this] {return Information();});
}

void Surface::MultiSubworldSampler::SetSubworld(
    SubworldGrid<MultipleSubworld> *subworldGrid) {
  fSubworld = subworldGrid;
  fLogger.WriteInfo("SubworldGrid set");
}

std::string Surface::MultiSubworldSampler::Information() const {
  std::stringstream ss;
  ss << "\n";
  ss << "\n";
  ss << "**************************************************\n";
  ss << "*        Information MultiSubworldSampler        *\n";
  ss << "**************************************************\n";
  ss << "\n";
  ss << "Subworlds: (Name) , (FacetStoreName) , (SurfaceArea), (Placement)\n";
  ss << "\n";

  const std::set<MultipleSubworld *> unique = fSubworld->GetUniqueSubworlds();
  std::map<MultipleSubworld *, G4double> surfaceArea;
  for (auto *subworld : unique) {
    FacetStore *facetStore = subworld->GetFacetStore();
    Calculator calc(facetStore);
    const G4ThreeVector facetStoreTrafo = facetStore->GetTransformation();
    ss << std::setw(20) << std::right << subworld->GetName()
       << " FacetStore: " << std::setw(20) << facetStore->GetStoreName()
       << " Area: " << calc.GetArea() / (CLHEP::mm * CLHEP::mm)
       << " mm^2 Placement:" << facetStoreTrafo.x() << " "
       << facetStoreTrafo.y() << " " << facetStoreTrafo.z() << "\n";
  }
  ss << "\n";
  ss << fSubworld->StreamStatistic().str();
  ss << "\n";
  ss << fSubworld->StreamLegend().str();
  ss << "\n";
  ss << fSubworld->StreamGrid(0, fSubworld->MaxX(), 0, fSubworld->MaxY()).str();
  ss << "\n";
  ss << "**************************************************\n";
  ss << "**************************************************\n";
  return ss.str();
}
