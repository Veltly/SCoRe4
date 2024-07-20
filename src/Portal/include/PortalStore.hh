// Copyright [2024] C.Gruener
// Date: 24-05-24
// File: PortalStore

#ifndef SRC_PORTAL_INCLUDE_PORTALSTORE_HH_
#define SRC_PORTAL_INCLUDE_PORTALSTORE_HH_

#include <vector>

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/VPortal.hh"

namespace Surface {

class PortalStore : public std::vector<VPortal *> {
 public:
  G4bool IsPortal(const G4VPhysicalVolume *volume) const;
  G4bool IsLogPortal(const G4LogicalVolume *volume) const;
  G4bool IsNotPortal(const G4VPhysicalVolume *volume) const;
  G4bool IsTrigger(const G4VPhysicalVolume *volume) const;

  G4int FindPortal(const G4VPhysicalVolume *volume) const;
  G4int FindPortalId(const G4String &name) const;
  G4int FindLogPortal(const G4LogicalVolume *volume) const;
  G4int FindTrigger(const G4VPhysicalVolume *volume) const;

  VPortal *GetPortal(const G4VPhysicalVolume *volume) const;
};
}  // namespace Surface

#endif  // SRC_PORTAL_INCLUDE_PORTALSTORE_HH_
