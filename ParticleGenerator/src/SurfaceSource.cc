//
//
//
//
//      Author: C.Gruener
//
//

#include "../include/SurfaceSource.hh"
#include "../include/SurfaceSourceMessenger.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

Surface::SurfaceSource::SurfaceSource()
    : fParticleGenerator(new G4GeneralParticleSource),
      Messenger(new SurfaceSourceMessenger(this)) {}

Surface::SurfaceSource::~SurfaceSource() {
  delete fParticleGenerator;
  fParticleGenerator = nullptr;
}

void Surface::SurfaceSource::SetFacetStore(Surface::FacetStore &aFacetStore) {
  fFacetStore = aFacetStore;
}

void Surface::SurfaceSource::GeneratePrimaryVertex(G4Event *argEvent) {

  fParticleGenerator->GeneratePrimaryVertex(argEvent);
  for (int i = 0; i < argEvent->GetNumberOfPrimaryVertex(); i++) {
    auto randomPoint = fFacetStore.GetRandomPoint();
    argEvent->GetPrimaryVertex(i)->SetPosition(randomPoint.x(), randomPoint.y(),
                                               randomPoint.z());
  }
}
