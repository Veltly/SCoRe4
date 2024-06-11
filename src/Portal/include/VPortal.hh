// Author: C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#ifndef VPORTAL_HH
#define VPORTAL_HH

#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <cstdlib>
namespace Surface {

enum class PortalType {
  SimplePortal,
  PeriodicPortal,
  PeriodicMultipleSubworld
};

class VPortal {
protected:
  G4ThreeVector GetLocalCoordSystem(G4VPhysicalVolume *volume);
  G4ThreeVector GetLocalCoordSystem();

  G4ThreeVector TransformToLocalCoordinate(const G4ThreeVector &vec);
  G4ThreeVector TransformToGlobalCoordinate(const G4ThreeVector &vec);
  void UpdatePosition(const G4Step *step, G4ThreeVector &newPosition);
  void UpdatePositionMomentum(const G4Step *step, G4ThreeVector &newPosition,
                              G4ThreeVector &newDirection);

public:
  virtual void DoPortation(const G4Step *) {
    exit(EXIT_FAILURE);
  }; // Function must be overwritten
  VPortal(G4String, G4VPhysicalVolume *, PortalType);
  VPortal(G4String, G4VPhysicalVolume *, PortalType,
          G4ThreeVector &globalCoord);
  // Getter
  inline G4VPhysicalVolume *GetVolume() const { return fVolume; };
  inline G4String GetName() const { return fName; }
  inline PortalType GetPortalType() const { return fPortalType; };
  inline G4VPhysicalVolume *GetTrigger() const { return fTrigger; };
  // Setter
  void SetGlobalCoord(G4ThreeVector vec);
  void SetVerbose(G4int verbose);
  void SetTrigger(G4VPhysicalVolume *volume);

private:
  G4String fName;
  G4VPhysicalVolume *fVolume;
  PortalType fPortalType;
  G4ThreeVector fGlobalCoord;
  G4bool fGlobalCoordSet;
  G4VPhysicalVolume *fTrigger;
};
} // namespace Surface
#endif // VPORTAL_HH
