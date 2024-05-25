// Author: C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#include "../include/VPortal.hh"
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4VPhysicalVolume.hh>

Surface::VPortal::VPortal(G4String name, G4VPhysicalVolume *volume, G4ThreeVector &globalCoord)
    : fName(name), fVolume(volume), fGlobalCoord(globalCoord){};

G4ThreeVector Surface::VPortal::GetLocalCoordSystem(G4VPhysicalVolume *volume){

}

G4ThreeVector Surface::VPortal::GetLocalCoordSystem(){
    return GetLocalCoordSystem(fVolume);
}


