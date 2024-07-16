// Copyright [2024] C.Gruener
// Date: 24-05-25
// File: PortalControl

#ifndef SRC_PORTAL_INCLUDE_PORTALCONTROL_HH_
#define SRC_PORTAL_INCLUDE_PORTALCONTROL_HH_

#include <G4VPhysicalVolume.hh>

#include "G4UserSteppingAction.hh"
#include "Portal/include/PortalStore.hh"
#include "Service/include/Logger.hh"
namespace Surface {

class PortalControl {
 public:
  PortalControl();
  PortalControl(const G4int verboseLvl);
  void DoStep(G4Step *step);
  void DoStep(const G4Step *step);
  void DoPortation(G4Step *step, const G4VPhysicalVolume *volume);
  void EnterPortal(G4Step *step);
  void LeavePortal(G4Step *step);
  G4bool EnterPortalCheck(const G4Step *step);
  void SetVerbose(const G4int verboseLvl);
  G4bool IsVolumeInsidePortal(const G4VPhysicalVolume *volume) const;
  void UsePortal(G4Step *step);

 private:
  PortalStore &fPortalStore;
  Logger fLogger;
  G4bool fJustPorted;
  G4bool fInSubworld;
  G4StepPoint fRecentStepPoint;
  G4bool fInPortal;
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_PORTALCONTROL_HH_
