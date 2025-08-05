/**
 * @brief definition of RunAction class
 * @file RunAction.hh
 * @date 2025-08-05
 */

#ifndef RunAction_surfaceShift_hh
#define RunAction_surfaceShift_hh

#include "G4UserRunAction.hh"

class RunAction : public G4UserRunAction {
 public:
  RunAction();
  ~RunAction() override;
};

#endif
