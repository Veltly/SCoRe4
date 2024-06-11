// Author: C.Gruener
// Date: 24-06-11
// File: PeriodicMultipleSubworlds

#ifndef PERIODICPORTAL_HH
#define PERIODICPORTAL_HH

#include "../../Service/include/Logger.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "SubworldGrid.hh"
#include "VPortal.hh"
#include <G4ThreeVector.hh>

namespace Surface {

class PeriodicMultipleSubworld : public VPortal {

  enum class SingleSurface {
    X_UP,
    X_DOWN,
    Y_UP,
    Y_DOWN,
    X_UP_Y_UP,
    X_UP_Y_DOWN,
    X_DOWN_Y_UP,
    X_DOWN_Y_DOWN,
    Z_UP,
    Z_DOWN
  };
  enum class PortationType { ENTER, EXIT, PERIODIC };

public:
  PeriodicMultipleSubworld(const G4String name, G4VPhysicalVolume *volume,
                           G4ThreeVector &vec, const G4int verbose);
  ~PeriodicMultipleSubworld();
  virtual void DoPortation(const G4Step *step);
  // Setter
  void SetGrid(const int nX, const int nY);
  void AddSubworldToGrid(const G4int x, const G4int y,
                         PeriodicMultipleSubworld *subworld);
  void SetAsPortal() { fIsPortal = true; }
  void SetOtherPortal(PeriodicMultipleSubworld *otherPortal);
  // Getter
  SubworldGrid<PeriodicMultipleSubworld> *GetSubworldGrid() const {
    return fSubworldGrid;
  }
  // Check
  inline G4bool IsPortal() const { return fIsPortal; }

private:
  void DoPeriodicPortation(const G4Step *step, const SingleSurface);
  void EnterPortal(const G4Step *step, const SingleSurface);
  void ExitPortal(const G4Step *step, const SingleSurface);

  SingleSurface GetNearestSurface(const G4Step *step);
  PortationType GetPortationType(const SingleSurface) const;

  void DoPeriodicTransform(G4ThreeVector &vec, SingleSurface);
  void TransformSubworldToPortal(G4ThreeVector &vec);
  void TransformPortalToSubworld(G4ThreeVector &vec);
  G4double TransformZBetweenPortals(const G4double val);

private:
  Logger fLogger;
  PeriodicMultipleSubworld *fPortal;
  G4VPhysicalVolume *fTrigger;
  G4bool fIsPortal; // True if it is the portal, false if it is the periodic
                    // subworld
  SubworldGrid<PeriodicMultipleSubworld> *fSubworldGrid;
  // order of Grid such that (fCurrentNX, fCurrentNy) = (0,0)
  // has the lowest local (x,y) coordinate in the portal
  // Grid will only be initialized if called
};
} // namespace Surface
#endif // PERIODICPORTAL_HH
