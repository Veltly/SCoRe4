//
//
//
//
//      Author: C.Gruener
//
//

#include "../include/SurfaceSource.hh"
#include "../../Service/include/Locator.hh"
#include "../include/SurfaceSourceMessenger.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4TriangularFacet.hh"
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

void Surface::SurfaceSource::LogSurface(G4String &aFilename) {
  fFacetStore.LogFacetStore(aFilename);
}
