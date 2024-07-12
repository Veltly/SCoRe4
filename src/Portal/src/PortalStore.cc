// Author: C.Gruener
// Date: 24-05-24
// File: PortalStore

#include "../include/PortalStore.hh"
#include "../include/VPortal.hh"
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <algorithm>

G4bool Surface::PortalStore::IsPortal(const G4VPhysicalVolume *volume) const {
  G4int portalIdx = FindPortal(volume);
  return portalIdx >= 0;
}

G4bool
Surface::PortalStore::IsNotPortal(const G4VPhysicalVolume *volume) const {
  return !IsPortal(volume);
}

G4int Surface::PortalStore::FindPortal(const G4VPhysicalVolume *volume) const {
  for (size_t i = 0; i < this->size(); ++i) {
    if (this->at(i)->GetVolume() == volume) {
      return static_cast<G4int>(i);
    }
  }
  return -1;
}

G4int Surface::PortalStore::FindPortalId(const G4String &name) const {
  for (size_t i = 0; i < this->size(); ++i) {
    if (this->at(i)->GetName() == name) {
      return static_cast<G4int>(i);
    }
  }
  return -1;
}

Surface::VPortal *
Surface::PortalStore::GetPortal(const G4VPhysicalVolume *volume) const {
  G4int portalIdx = FindPortal(volume);
  return this->at(portalIdx);
}

G4int Surface::PortalStore::FindLogPortal(const G4LogicalVolume *volume) const {
  for (size_t i = 0; i < this->size(); ++i) {
    if (this->at(i)->GetVolume()->GetLogicalVolume() == volume) {
      return static_cast<G4int>(i);
    }
  }
  return -1;
}

G4bool
Surface::PortalStore::IsLogPortal(const G4LogicalVolume *logVolume) const {
  G4int portalIdx = FindLogPortal(logVolume);
  return portalIdx >= 0;
}

G4int Surface::PortalStore::FindTrigger(const G4VPhysicalVolume *volume) const {
  for (size_t i = 0; i < this->size(); ++i) {
    if (this->at(i)->GetTrigger() == volume) {
      return static_cast<G4int>(i);
    }
  }
  return -1;
}
G4bool Surface::PortalStore::IsTrigger(const G4VPhysicalVolume *volume) const {
  G4int portalIdx = FindTrigger(volume);
  return portalIdx >= 0;
}
