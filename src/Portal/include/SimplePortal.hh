// Author: C.Gruener
// Date: 24-05-24
// File: SimplePortal

// Simple Portal combines two Boxes
#ifndef SIMPLE_PORTAL_HH
#define SIMPLE_PORTAL_HH
#include "../../Service/include/Logger.hh"
#include "VPortal.hh"
#include <G4ThreeVector.hh>
#include <G4VPhysicalVolume.hh>

namespace Surface {

class SimplePortal : public VPortal {
private:
  G4ThreeVector TransformBetweenPortals(G4ThreeVector &vec);

public:
  SimplePortal(G4String name, G4VPhysicalVolume *volume, G4ThreeVector &vec,
               G4int verbose)
      : VPortal(name, volume, PortalType::SimplePortal, vec),
        fLogger({"SimplePortal" + name, verbose}){};
  virtual void DoPortation(G4StepPoint *point) override;
  void SetOtherPortal(SimplePortal *otherPortal);
  void SetVerbose(G4int verbose);

private:
  SimplePortal *fOtherPortal;
  Logger fLogger;
};

} // namespace Surface
#endif // SIMPLE_PORTAL_HH
