/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"


#include "Portal/include/PortalControl.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

SteppingAction::SteppingAction() : G4UserSteppingAction() {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step *step) {
  fPortalControl.DoStep(step);
}
