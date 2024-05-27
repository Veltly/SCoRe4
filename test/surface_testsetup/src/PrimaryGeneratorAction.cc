#include "PrimaryGeneratorAction.hh"
#include "../../../src/ParticleGenerator/include/SurfaceSource.hh"
#include "../../../src/Service/include/Locator.hh"
#include "../../../src/SurfaceGenerator/include/FacetStore.hh"
#include "G4RunManager.hh"
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fSurfaceSource(new Surface::SurfaceSource()) {}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fSurfaceSource; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fSurfaceSource->GeneratePrimaryVertex(anEvent);
}
