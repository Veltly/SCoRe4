//
// Created by chris on 01.11.25.
//

#ifndef EXAMPLE_SURFACE_RUNACTION_HH
#define EXAMPLE_SURFACE_RUNACTION_HH

#include "G4UserRunAction.hh"

class RunAction : public G4UserRunAction {
 public:
  ~RunAction() override;

  void BeginOfRunAction(const G4Run*) override;
  void EndOfRunAction(const G4Run*) override;
};

#endif  // EXAMPLE_SURFACE_RUNACTION_HH
