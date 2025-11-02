//
// Created by chris on 01.11.25.
//
#ifndef SURFACE_EVENTACTION_HH
#define SURFACE_EVENTACTION_HH

#include "G4UserEventAction.hh"

class EventAction : G4UserEventAction {
  EventAction();
  ~EventAction();

  void EndOfEventAction(const G4Event*) override;
};

#endif  // SURFACE_EVENTACTION_HH
