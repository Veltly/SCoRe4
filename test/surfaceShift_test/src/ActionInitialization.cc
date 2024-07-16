
#include "ActionInitialization.hh"

#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);

  RunAction *runAction = new RunAction;
  SetUserAction(runAction);

  EventAction *eventAction = new EventAction();
  SetUserAction(eventAction);
}
