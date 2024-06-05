// Author: C.Gruener
// Date: 04-06-24
// File: PeriodicPortal

#ifndef PERIODICPORTAL_HH
#define PERIODICPORTAL_HH

#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "VPortal.hh"
#include <G4ThreeVector.hh>

namespace Surface {

class PeriodicPortal : public VPortal {
public:
  PeriodicPortal(G4String name, G4VPhysicalVolume *volume, G4ThreeVector &vec,
                 G4int verbose);
  virtual void DoPortation(const G4Step *step);

private:
  void DoPeriodicPortation();
  void EnterPortal();
  void LeavePortal();
  void GetExitSurface(const G4Step *step);

private:
  PeriodicPortal *fOtherPortal;
  G4VPhysicalVolume *fTrigger;
  G4int fNX;
  G4int fNY;
};
} // namespace Surface
#endif // PERIODICPORTAL_HH
