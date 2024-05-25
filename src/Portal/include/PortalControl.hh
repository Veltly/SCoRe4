//Author: C.Gruener
//Date: 24-05-25
//File: PortalControl


#ifndef PORTALCONTROL_HH
#define PORTALCONTROL_HH

#include "PortalStore.hh"
#include "G4UserSteppingAction.hh"
namespace Surface{

class PortalControl{
public:
  PortalControl();
  void DoStep(G4Step *step);
private:
  PortalStore &fPortalStore;
};
}
#endif //PORTALCONTROL_HH
