#include "PrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4RunManager.hh"
#include "ParticleGenerator/include/ShiftTable.hh"
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4ThreeVector.hh>

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), fGps(new G4GeneralParticleSource()),
      fShiftTable(new Surface::Shift{3}) {}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fGps;
  delete fShiftTable;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fGps->GeneratePrimaryVertex(anEvent);
  G4ThreeVector position = anEvent->GetPrimaryVertex()->GetPosition();
  G4ThreeVector direction{0., 0., -1.};
  fShiftTable->DoShift(position, direction);
  anEvent->GetPrimaryVertex()->SetPosition(position.x(), position.y(),
                                           position.z());
}
