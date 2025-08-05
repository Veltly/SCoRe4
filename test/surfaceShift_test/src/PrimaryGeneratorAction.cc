/**
* @brief definition of the PrimaryGeneratorAction class using the shift class
* @file PrimaryGeneratorAction.cc
* @date 2025-08-05
*/

#include "ParticleGenerator/include/ShiftTable.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4RunManager.hh"
#include <G4ThreeVector.hh>

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), fGps(new G4GeneralParticleSource()),
      fShiftTable(new Surface::Shift{3}) {}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fGps;
  delete fShiftTable;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  // generates primary vertex
  fGps->GeneratePrimaryVertex(anEvent);
  // get the current position of generated vertex
  G4ThreeVector position = anEvent->GetPrimaryVertex()->GetPosition();
  // set shift-direction
  G4ThreeVector shiftDirection{0., 0., -1.};
  // shift the current position
  fShiftTable->DoShift(position, shiftDirection);
  //set the new position of the generated primary vertex
  anEvent->GetPrimaryVertex()->SetPosition(position.x(), position.y(),
                                           position.z());
}
