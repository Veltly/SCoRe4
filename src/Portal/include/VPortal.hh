// Author: C.Gruener
// Date: 24-05-24
// File: VirtualPortal

#ifndef VPORTAL_HH
#define VPORTAL_HH

#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"

namespace Surface {

class VPortal {
public:
  VPortal(G4String, G4VPhysicalVolume *);
  inline G4VPhysicalVolume *GetVolume() const { return volume; };
  inline G4String GetName() const { return name; }
  virtual void DoPortation(G4StepPoint &point) = 0;

private:
  G4String name;
  G4VPhysicalVolume *volume;
};
} // namespace Surface
#endif // VPORTAL_HH
