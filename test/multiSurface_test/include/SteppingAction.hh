/**
 * @brief implementation of user SteppingAction: check for portation each step\
 * @file SteppingAction.hh
 * @date 2025-08-04
 * @author C.Gruener
 */

#ifndef SteppingAction_MultiSurface_h
#define SteppingAction_MultiSurface_h

#include "../../../src/Portal/include/PortalControl.hh"
#include "G4UserSteppingAction.hh"

class SteppingAction : public G4UserSteppingAction {
 public:
  SteppingAction();
  ~SteppingAction() override;

  void UserSteppingAction(const G4Step *) override;

 private:
  Surface::PortalControl fPortalControl{}; // class controls usage of portal
};

#endif
