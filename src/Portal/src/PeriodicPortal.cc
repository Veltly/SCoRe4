// Author: C.Gruener
// Date: 04-06-025
// File: PeriodicPortal

#include "../include/PeriodicPortal.hh"
#include "../include/VPortal.hh"
#include "G4Box.hh"
#include "G4Step.hh"
#include <G4ThreeVector.hh>
#include <G4VPhysicalVolume.hh>

enum class SingleSurface { X_UP, X_DOWN, Y_UP, Y_DOWN, Z_UP, Z_DOWN };

Surface::PeriodicPortal::PeriodicPortal(G4String name,
                                        G4VPhysicalVolume *volume,
                                        G4ThreeVector &vec, G4int verbose)
    : VPortal(name, volume, PortalType::PeriodicPortal) {
  SetVerbose(verbose);
  SetGlobalCoord(vec);
};

void Surface::PeriodicPortal::DoPortation(const G4Step *step){
    // check if Portation is possible (change of volume)

    // check if Portation is triggered( postStepPoint in trigger)

    // select Portation Method by checking Grid and side of trigger

    // -->case PeriodicPortation
    // update Grid

    // -->case LeavePortal

};

// Function to decide in which direction the particle left the volume
// use G4ThreeVector result = solid->SurfaceNormal(point);
void Surface::PeriodicPortal::GetExitSurface(const G4Step *step) {
  G4VSolid *portalSolid = GetVolume()->GetLogicalVolume()->GetSolid();
  G4ThreeVector result =
      portalSolid->SurfaceNormal(step->GetPostStepPoint()->GetPosition());
  // switch
  // case
}
