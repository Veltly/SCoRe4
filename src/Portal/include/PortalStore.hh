// Author: C.Gruener
// Date: 24-05-24
// File: PortalStore

#ifndef PORTALSTORE_HH
#define PORTALSTORE_HH

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "VPortal.hh"

namespace Surface {

class PortalStore : public std::vector<VPortal *> {
private:
public:
  G4int FindPortal(const G4VPhysicalVolume *volume) const;
  G4int FindLogPortal(const G4LogicalVolume *volume) const;
  G4int FindTrigger(const G4VPhysicalVolume *volume) const;
  G4bool IsPortal(const G4VPhysicalVolume *volume) const;
  G4bool IsLogPortal(const G4LogicalVolume *volume) const;
  G4bool IsNotPortal(const G4VPhysicalVolume *volume) const;
  VPortal *GetPortal(const G4VPhysicalVolume *volume) const;
  G4bool IsTrigger(const G4VPhysicalVolume *volume) const;
};
} // namespace Surface

#endif // PORTALSTORE_HH
