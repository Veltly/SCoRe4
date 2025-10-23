#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fSurfaceSource(new Surface::SurfaceSource()) {}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fSurfaceSource; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fSurfaceSource->GeneratePrimaryVertex(anEvent);
}
