/**
 * @brief Simple portal is a portal between two boxlike volumes
 * @file SimplePortal.hh
 * @author C.Gruener
 * @date 24-05-24
 */

#ifndef SRC_PORTAL_INCLUDE_SIMPLEPORTAL_HH
#define SRC_PORTAL_INCLUDE_SIMPLEPORTAL_HH

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
               const G4ThreeVector &vec, const VerboseLevel verbose = VerboseLevel::Default)
      : VPortal("SimplePortal_" + name, volume, PortalType::SimplePortal, vec, verbose){}

  void DoPortation(G4Step *step) override;
  void SetOtherPortal(SimplePortal *otherPortal);

 private:
  SimplePortal *fOtherPortal{nullptr};
};

}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_SIMPLEPORTAL_HH
