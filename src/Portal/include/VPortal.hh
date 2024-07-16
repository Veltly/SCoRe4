// Copyright [2024] C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#ifndef SRC_PORTAL_INCLUDE_VPORTAL_HH_
#define SRC_PORTAL_INCLUDE_VPORTAL_HH_

#include <cstdlib>

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "Service/include/Logger.hh"

namespace Surface {

enum class PortalType { SimplePortal, PeriodicPortal, MultipleSubworld };

class VPortal {
 public:
  virtual void DoPortation(G4Step *) {
    exit(EXIT_FAILURE);
  }  // Function must be overwritten
  //
  VPortal(const G4String &name, G4VPhysicalVolume *, PortalType);
  VPortal(const G4String &name, G4VPhysicalVolume *, PortalType,
          const G4ThreeVector &globalCoord);
  // Getter
  inline G4VPhysicalVolume *GetVolume() const { return fVolume; }
  inline G4String GetName() const { return fName; }
  inline PortalType GetPortalType() const { return fPortalType; }
  inline G4VPhysicalVolume *GetTrigger() const { return fTrigger; }
  // Setter
  void SetGlobalCoord(G4ThreeVector vec);
  void SetVerbose(G4int verbose);
  void SetTrigger(G4VPhysicalVolume *volume);

 protected:
  // TODO: G4ThreeVector GetLocalCoordSystem(G4VPhysicalVolume *volume);
  G4ThreeVector GetLocalCoordSystem();

  void TransformToLocalCoordinate(G4ThreeVector &vec);
  void TransformToGlobalCoordinate(G4ThreeVector &vec);
  void UpdatePosition(const G4Step *step, G4ThreeVector &newPosition);
  void UpdatePositionMomentum(const G4Step *step, G4ThreeVector &newPosition,
                              G4ThreeVector &newDirection);

 private:
  G4String fName;
  Logger fLogger;
  G4VPhysicalVolume *fVolume;
  PortalType fPortalType;
  G4ThreeVector fGlobalCoord;
  G4bool fGlobalCoordSet;
  G4VPhysicalVolume *fTrigger;
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_VPORTAL_HH_
