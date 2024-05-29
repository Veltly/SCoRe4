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
  void DoPortation(G4StepPoint *stepPoint, G4VPhysicalVolume *volume);
  void DoPortation(const G4Step *step, G4VPhysicalVolume *volume);
  void SetVerbose(G4int verbose);

private:
  PortalStore &fPortalStore;
  Logger fLogger;
  G4bool fJustPorted;
};
} // namespace Surface
#endif // PORTALCONTROL_HH
