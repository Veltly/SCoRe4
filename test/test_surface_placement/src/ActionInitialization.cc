/**
* @brief definition of userAction (PrimaryGeneratorAction, runAction and eventAction)
* @file ActionInitialization.cc
* @date 2025-08-05
*/

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::BuildForMaster() const {}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);

//  auto *runAction = new RunAction;
//  SetUserAction(runAction);
//
//  auto *eventAction = new EventAction();
//  SetUserAction(eventAction);
}
