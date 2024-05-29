// Author: C.Gruener
// Date: 24-05-24
// File: PortalStore

#ifndef PORTALSTORE_HH
#define PORTALSTORE_HH

#include "G4VPhysicalVolume.hh"
#include "VPortal.hh"

namespace Surface {

class PortalStore : public std::vector<VPortal *> {
private:
  G4int FindPortal(const G4VPhysicalVolume *volume) const;

public:
  G4bool IsPortal(const G4VPhysicalVolume *volume) const;
  G4bool IsNotPortal(const G4VPhysicalVolume *volume) const;
  VPortal *GetPortal(const G4VPhysicalVolume *volume) const;
};
} // namespace Surface

#endif // PORTALSTORE_HH
