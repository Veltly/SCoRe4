// Author: C.Gruener
// Date: 24-05-25
// File: PortalControl

#ifndef PORTALCONTROL_HH
#define PORTALCONTROL_HH

#include "../../Service/include/Logger.hh"
#include "G4UserSteppingAction.hh"
#include "PortalStore.hh"
#include <G4VPhysicalVolume.hh>
namespace Surface {

class PortalControl {
public:
  PortalControl(G4int verbose = 3);
  void DoStep(const G4Step *step);
  void DoPortation(const G4Step *step, G4VPhysicalVolume *volume);
  void EnterPortal(const G4Step *step);
  void LeavePortal(const G4Step *step);
  G4bool EnterPortalCheck(const G4Step *step);
  void SetVerbose(G4int verbose);
  G4bool IsVolumeInsidePortal(const G4VPhysicalVolume *volume) const;
  void UsePortal(const G4Step *step);

private:
  PortalStore &fPortalStore;
  Logger fLogger;
  G4bool fJustPorted;
  G4bool fInSubworld;
  G4StepPoint fRecentStepPoint;
  G4bool fInPortal;
};
} // namespace Surface
#endif // PORTALCONTROL_HH
