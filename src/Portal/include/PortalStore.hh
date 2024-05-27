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
  G4int FindPortal(G4VPhysicalVolume *volume);

public:
  G4bool IsPortal(G4VPhysicalVolume *volume);
  VPortal *GetPortal(G4VPhysicalVolume *volume);
};
} // namespace Surface

#endif // PORTALSTORE_HH
