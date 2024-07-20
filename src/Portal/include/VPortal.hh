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
  virtual void DoPortation(G4Step *) = 0;  //

  VPortal(const G4String &name, G4VPhysicalVolume *, const PortalType);
  VPortal(const G4String &name, G4VPhysicalVolume *, const PortalType,
          const G4ThreeVector &globalCoord);
  // Getter
  inline G4VPhysicalVolume *GetVolume() const { return fVolume; }
  inline G4String GetName() const { return fName; }
  inline PortalType GetPortalType() const { return fPortalType; }
  inline G4VPhysicalVolume *GetTrigger() const { return fTrigger; }
  // Setter
  void SetGlobalCoord(const G4ThreeVector vec);
  void SetVerbose(const G4int verboseLvl);
  void SetTrigger(G4VPhysicalVolume *volume);

 protected:
  G4ThreeVector GetLocalCoordSystem() const;

  void TransformToLocalCoordinate(G4ThreeVector &vec);
  void TransformToGlobalCoordinate(G4ThreeVector &vec);
  void UpdatePosition(G4Step *step, const G4ThreeVector &newPosition);
  void UpdatePositionMomentum(G4Step *step, const G4ThreeVector &newPosition,
                              const G4ThreeVector &newDirection);

 private:
  const G4String fName;
  Logger fLogger;
  G4VPhysicalVolume *fVolume;
  const PortalType fPortalType;
  G4ThreeVector fGlobalCoord;
  G4bool fGlobalCoordSet;
  G4VPhysicalVolume *fTrigger;
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_VPORTAL_HH_
