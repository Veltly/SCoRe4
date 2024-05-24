// Author: C.Gruener
// Date: 24-05-24
// File: PortalStore

#include "Portal/include/PortalStore.hh"
#include "Portal/include/VPortal.hh"
#include <algorithm>

G4bool Surface::PortalStore::IsPortal(G4VPhysicalVolume *volume) {}

G4int Surface::PortalStore::FindPortal(G4VPhysicalVolume *volume) {
  auto compare = [&volume](VPortal &portal) {
    return portal.GetVolume() == volume;
  };
  auto it = std::find_if(this->cbegin(), this->cend(), compare);
    if(it
}
