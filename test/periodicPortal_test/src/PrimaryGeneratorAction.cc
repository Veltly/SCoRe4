#include "PrimaryGeneratorAction.hh"
#include "../../../src/ParticleGenerator/include/SurfaceSource.hh"
#include "../../../src/Service/include/Locator.hh"
#include "../../../src/SurfaceGenerator/include/FacetStore.hh"
#include "G4RunManager.hh"
#include <G4GeneralParticleSource.hh>
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), fGps(new G4GeneralParticleSource()) {}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fGps; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fGps->GeneratePrimaryVertex(anEvent);
}
