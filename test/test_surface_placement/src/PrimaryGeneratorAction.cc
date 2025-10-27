#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), f_surface_source(new Surface::Source("SurfaceSource",Surface::VerboseLevel::DebugInfo)){
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()= default;

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  f_surface_source->GeneratePrimaryVertex(anEvent);
}