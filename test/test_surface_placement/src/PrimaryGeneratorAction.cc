#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(){}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fSurfaceSource.GeneratePrimaryVertex(anEvent);
}

//PrimaryGeneratorAction::PrimaryGeneratorAction()
//    : G4VUserPrimaryGeneratorAction(){}
//
//PrimaryGeneratorAction::~PrimaryGeneratorAction() { }
//
//void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
//;
//}
