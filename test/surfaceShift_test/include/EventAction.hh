/**
 * @brief Definition of userEventAction class
 * @file EventAction.hh
 * @date 2025-08-05
 */

#ifndef EventAction_surfaceShift_hh
#define EventAction_surfaceShift_hh

#include "G4UserEventAction.hh"

class RunAction;

class EventAction : public G4UserEventAction {
 public:
  EventAction();
  ~EventAction() override;

  void BeginOfEventAction(const G4Event *event) override;
  void EndOfEventAction(const G4Event *event) override;

 private:
};

#endif
