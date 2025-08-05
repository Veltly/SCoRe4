/**
 * @brief definition of the PrimaryGeneratorAction class for sampling of vertices
 * @file PrimaryGeneratorAction.cc
 * @date 2025-08-04
 * @author C.Gruener
 */

#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include <G4GeneralParticleSource.hh>

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fSampler(Surface::MultiSubworldSampler{"MultiSurfaceSampler", // name of sampler
                                             "MultiSurfacePortal", // name of portal
                                             "macros/shiftTable"}) {} // path to shift-table

PrimaryGeneratorAction::~PrimaryGeneratorAction() = default;

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fSampler.GeneratePrimaryVertex(anEvent); // sample start point for simulation
}
