// Copyright [2024] C.Gruener
// Date: 24-05-24
// File: SimplePortal

// Simple Portal combines two Boxes
#ifndef SRC_PORTAL_INCLUDE_SIMPLEPORTAL_HH_
#define SRC_PORTAL_INCLUDE_SIMPLEPORTAL_HH_

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/VPortal.hh"
#include "Service/include/Logger.hh"

namespace Surface {

class SimplePortal : public VPortal {
 private:
  G4ThreeVector TransformBetweenPortals(const G4ThreeVector &vec);

 public:
  SimplePortal(const G4String &name, G4VPhysicalVolume *volume,
               const G4ThreeVector &vec, const G4int verbose)
      : VPortal(name, volume, PortalType::SimplePortal, vec),
        fLogger("SimplePortal_" + name, verbose) {}

  void DoPortation(G4Step *step) override;
  void SetOtherPortal(SimplePortal *otherPortal);
  void SetVerbose(G4int verbose);

 private:
  SimplePortal *fOtherPortal;
  Logger fLogger;
};

}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_SIMPLEPORTAL_HH_
