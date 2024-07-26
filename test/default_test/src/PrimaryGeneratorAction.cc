#include "PrimaryGeneratorAction.hh"

#include <G4GeneralParticleSource.hh>

#include "../../../src/ParticleGenerator/include/SurfaceSource.hh"
#include "G4RunManager.hh"
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fSampler(Surface::MultiSubworldSampler{"Testsampler",
                                             "MultiplePortal_Portal"}) {}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fSampler.GeneratePrimaryVertex(anEvent);
}
