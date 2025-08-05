/**
* @brief set UserAction (PrimaryGeneratorAction and SteppingAction)
* @file ActionInitialization.cc
* @date 2025-08-05
*/

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::BuildForMaster() const {}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);
  G4cout << "Stepping action initialized\n";
  SetUserAction(new SteppingAction());
}
