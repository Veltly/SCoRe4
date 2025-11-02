/**
* @brief Implementation of Primary generator action class
* @file PrimaryGeneratorAction.hh
* @date 2025-10-27
 */

#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      f_surface_source(new Surface::Source("SurfaceSource",
                                           nullptr,
                                           Surface::VerboseLevel::Default)
                       ){}

PrimaryGeneratorAction::~PrimaryGeneratorAction()= default;

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  f_surface_source->GeneratePrimaryVertex(anEvent);
}