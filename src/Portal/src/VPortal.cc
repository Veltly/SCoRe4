// Author: C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#include "../include/VPortal.hh"
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4VPhysicalVolume.hh>
#include <cstdlib>

Surface::VPortal::VPortal(G4String name, G4VPhysicalVolume *volume,
                          PortalType type)
    : fName(name), fVolume(volume), fPortalType(type), fGlobalCoordSet(false){};

Surface::VPortal::VPortal(G4String name, G4VPhysicalVolume *volume,
                          PortalType type, G4ThreeVector &globalCoord)
    : fName(name), fVolume(volume), fPortalType(type),
      fGlobalCoord(globalCoord), fGlobalCoordSet(true){};

G4ThreeVector Surface::VPortal::GetLocalCoordSystem(G4VPhysicalVolume *volume) {
  // To do: Implement routine to find global coordinate for given volume
  exit(EXIT_FAILURE);
}

G4ThreeVector Surface::VPortal::GetLocalCoordSystem() {
  if (fGlobalCoordSet) {
    return fGlobalCoord;
  }
  return GetLocalCoordSystem(fVolume);
}

void Surface::VPortal::SetGlobalCoord(G4ThreeVector vec) {
  fGlobalCoord = vec;
  fGlobalCoordSet = true;
}

G4ThreeVector
Surface::VPortal::TransformToLocalCoordinate(const G4ThreeVector &vec) {
  G4ThreeVector coordinateOfVolume = GetLocalCoordSystem();
  return vec - coordinateOfVolume;
}

G4ThreeVector
Surface::VPortal::TransformToGlobalCoordinate(const G4ThreeVector &vec) {
  G4ThreeVector coordinateOfVolume = GetLocalCoordSystem();
  return vec + coordinateOfVolume;
}
