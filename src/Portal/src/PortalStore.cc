/**
 * @brief Stores a handle to all portals
 * @file PortalStore.cc
 * @author C.Gruener
 * @date 24-05-24
 */
#include "Portal/include/PortalStore.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/VPortal.hh"

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

Surface::VPortal *Surface::PortalStore::GetPortal(
    const G4VPhysicalVolume *volume) const {
  const G4int portalIdx = FindPortal(volume);
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

G4bool Surface::PortalStore::IsLogPortal(
    const G4LogicalVolume *logVolume) const {
  const G4int portalIdx = FindLogPortal(logVolume);
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
  const G4int portalIdx = FindTrigger(volume);
  return portalIdx >= 0;
}
