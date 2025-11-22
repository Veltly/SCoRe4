/// \file SteppingAction.hh
/// \brief Definition of the SteppingAction class

#ifndef SURFACE_PORTAL_STEPPING_ACTION_HH
#define SURFACE_PORTAL_STEPPING_ACTION_HH

#include "Portal/include/PortalControl.hh"
#include "G4UserSteppingAction.hh"
#include "globals.hh"
class EventAction;

class G4LogicalVolume;

class SteppingAction : public G4UserSteppingAction {
 public:
  SteppingAction();
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step *);

 private:
  Surface::PortalControl fPortalControl{Surface::VerboseLevel::Default};
};

#endif //SURFACE_PORTAL_STEPPING_ACTION_HH
