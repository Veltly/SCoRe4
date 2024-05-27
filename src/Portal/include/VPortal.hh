// Author: C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#ifndef VPORTAL_HH
#define VPORTAL_HH

#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <cstdlib>

namespace Surface {

enum class PortalType { SimplePortal };

class VPortal {
protected:
  G4ThreeVector GetLocalCoordSystem(G4VPhysicalVolume *volume);
  G4ThreeVector GetLocalCoordSystem();

  G4ThreeVector TransformToLocalCoordinate(const G4ThreeVector &vec);
  G4ThreeVector TransformToGlobalCoordinate(const G4ThreeVector &vec);

public:
  virtual void DoPortation(G4StepPoint *point) {
    exit(EXIT_FAILURE); // Function must be overwritten
  };
  VPortal(G4String, G4VPhysicalVolume *, PortalType);
  VPortal(G4String, G4VPhysicalVolume *, PortalType,
          G4ThreeVector &globalCoord);
  // Getter
  inline G4VPhysicalVolume *GetVolume() const { return fVolume; };
  inline G4String GetName() const { return fName; }
  inline PortalType GetPortalType() const { return fPortalType; };
  // Setter
  void SetGlobalCoord(G4ThreeVector vec);

private:
  G4String fName;
  G4VPhysicalVolume *fVolume;
  PortalType fPortalType;
  G4ThreeVector fGlobalCoord;
  G4bool fGlobalCoordSet;
};
} // namespace Surface
#endif // VPORTAL_HH
