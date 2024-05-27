// Author: C.Gruener
// Date: 24-05-25
// File: PortalControl

#ifndef PORTALCONTROL_HH
#define PORTALCONTROL_HH

#include "G4UserSteppingAction.hh"
#include "PortalStore.hh"
#include <G4VPhysicalVolume.hh>
namespace Surface {

class PortalControl {
public:
  PortalControl();
  void DoStep(const G4Step *step);
  void DoPortation(G4StepPoint *stepPoint, G4VPhysicalVolume *volume);

private:
  PortalStore &fPortalStore;
};
} // namespace Surface
#endif // PORTALCONTROL_HH
