/**
* @brief Implementation of Primary generator action class
* @file PrimaryGeneratorAction.hh
* @date 2025-10-27
 */

#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "Surface/Shift.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      f_sampler(Surface::MultiSubworldSampler("Sampler",
                                              "MultipleSubworld_Portal")){}

PrimaryGeneratorAction::~PrimaryGeneratorAction()= default;

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  f_sampler.GeneratePrimaryVertex(anEvent);
}