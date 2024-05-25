// Author: C.Gruener
// Date: 24-05-24
// File: PortalStore

#include "Portal/include/PortalStore.hh"
#include "Portal/include/VPortal.hh"
#include <G4VPhysicalVolume.hh>
#include <algorithm>

G4bool Surface::PortalStore::IsPortal(G4VPhysicalVolume *volume) {
  G4int portalIdx = FindPortal(volume);
  return portalIdx >= 0;
}

G4int Surface::PortalStore::FindPortal(G4VPhysicalVolume *volume) {
  for(size_t i = 0; i < this->size(); ++i){
    if(this->at(i).GetVolume() == volume){
      return static_cast<G4int>(i);}
  }
  return -1;
}

Surface::VPortal &Surface::PortalStore::GetPortal(G4VPhysicalVolume *volume){
  G4int portalIdx = FindPortal(volume);
  return this->at(portalIdx);
}
