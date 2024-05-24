// Author: C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#include "../include/VPortal.hh"
#include <G4VPhysicalVolume.hh>

Surface::VPortal::VPortal(G4String name, G4VPhysicalVolume *volume)
    : name(name), volume(volume){};
