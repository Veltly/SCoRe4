// Author: C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#ifndef VPORTAL_HH
#define VPORTAL_HH

#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>

namespace Surface {

class VPortal {
protected:
  G4ThreeVector GetLocalCoordSystem(G4VPhysicalVolume *volume);
  G4ThreeVector GetLocalCoordSystem();

  G4ThreeVector TransformToLocalCoordinate(const G4ThreeVector &vec);
  G4ThreeVector TransformToGlobalCoordinate(const G4ThreeVector &vec);

public:
  VPortal(G4String, G4VPhysicalVolume *);
  VPortal(G4String, G4VPhysicalVolume *, G4ThreeVector &globalCoord);
  virtual void DoPortation(G4StepPoint *point);
  void SetGlobalCoord(G4ThreeVector vec);
  // Getter
  inline G4VPhysicalVolume *GetVolume() const { return fVolume; };
  inline G4String GetName() const { return fName; }

private:
  G4String fName;
  G4VPhysicalVolume *fVolume;
  G4ThreeVector fGlobalCoord;
  G4bool fGlobalCoordSet;
};
} // namespace Surface
#endif // VPORTAL_HH
