/**
 * @brief implementation of user SteppingAction: check for portation each step
 * @file SteppingAction.cc
 * @date 2025-08-04
 * @author C.Gruener
 */

#include "SteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

SteppingAction::SteppingAction() : G4UserSteppingAction() {}

SteppingAction::~SteppingAction() = default;

void SteppingAction::UserSteppingAction(const G4Step *step) {
  fPortalControl.DoStep(step); // check portal each step
}
