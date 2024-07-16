// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#include "ParticleGenerator/include/SurfaceSource.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "ParticleGenerator/include/SurfaceSourceMessenger.hh"
#include "Service/include/Locator.hh"
Surface::SurfaceSource::SurfaceSource()
    : fMessenger(new SurfaceSourceMessenger(this)),
      fParticleGenerator(new G4GeneralParticleSource) {}

Surface::SurfaceSource::~SurfaceSource() {
  delete fParticleGenerator;
  fParticleGenerator = nullptr;
}

void Surface::SurfaceSource::GeneratePrimaryVertex(G4Event *argEvent) {
  if (!fFacetStore.GetIsStoreClosed()) {
    fFacetStore = Locator::GetFacetStore();
    fFacetStore.CloseFacetStore();
  }
  fParticleGenerator->GeneratePrimaryVertex(argEvent);
  for (int i = 0; i < argEvent->GetNumberOfPrimaryVertex(); i++) {
    auto randomPoint = fFacetStore.GetRandomPoint();
    argEvent->GetPrimaryVertex(i)->SetPosition(randomPoint.x(), randomPoint.y(),
                                               randomPoint.z());
  }
}

void Surface::SurfaceSource::ShowSurface() { fFacetStore.DrawFacets(); }

void Surface::SurfaceSource::LogSurface(const G4String &aFilename) {
  fFacetStore.LogFacetStore(aFilename);
}
