// Author: C.Gruener
// Date: 24-06-04
// File: PeriodicPortal

#ifndef PERIODICPORTAL_HH
#define PERIODICPORTAL_HH

#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "VPortal.hh"
#include <G4ThreeVector.hh>

namespace Surface {

class PeriodicPortal : public VPortal {

  enum class SingleSurface { X_UP, X_DOWN, Y_UP, Y_DOWN, Z_UP, Z_DOWN };
  enum class PortationType { ENTER, EXIT, PERIODIC };

public:
  PeriodicPortal(G4String name, G4VPhysicalVolume *volume, G4ThreeVector &vec,
                 G4int verbose);
  virtual void DoPortation(const G4Step *step);
  void SetGrid(int nX, int nY);
  inline G4int GetMaxNX() const { return fMaxNX; }
  inline G4int GetMaxNY() const { return fMaxNY; }
  inline G4int GetCurrentNX() const { return fCurrentNX; }
  inline G4int GetCurrentNY() const { return fCurrentNY; }
  inline G4bool IsPortal() const { return fIsPortal; }
  void SetAsPortal() { fIsPortal = true; }

private:
  void DoPeriodicPortation(const G4Step *step, SingleSurface);
  void EnterPortal(const G4Step *step, SingleSurface);
  void ExitPortal(const G4Step *step, SingleSurface);
  SingleSurface GetNearestSurface(const G4Step *step) const;
  PortationType GetPortationType(SingleSurface) const;
  void DoPeriodicTransform(G4ThreeVector &vec, SingleSurface);
  void TransformSubworldToPortal(G4ThreeVector &vec);
  void TransformPortalToSubworld(G4ThreeVector &vec);
  G4double TransformZBetweenPortals(G4double val);

private:
  PeriodicPortal *fOtherPortal;
  G4VPhysicalVolume *fTrigger;
  G4bool fIsPortal; // True if it is the portal, false if it is the periodic
                    // subworld
  G4int fMaxNX;
  G4int fMaxNY;
  G4int fCurrentNX;
  G4int fCurrentNY;
  // order of Grid such that (fCurrentNX, fCurrentNy) = (0,0)
  // has the lowest local (x,y) coordinate in the portal
};
} // namespace Surface
#endif // PERIODICPORTAL_HH
