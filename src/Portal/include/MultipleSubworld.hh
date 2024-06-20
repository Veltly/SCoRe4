// Author: C.Gruener
// Date: 24-06-11
// File: MultipleSubworlds

#ifndef MULTIPLESUBWORLD_HH
#define MULTIPLESUBWORLD_HH

#include "../../Service/include/Logger.hh"
#include "../../SurfaceGenerator/include/FacetStore.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"
#include "SubworldGrid.hh"
#include "VPortal.hh"
#include <G4ThreeVector.hh>

namespace Surface {

class MultipleSubworld : public VPortal {

  enum class SingleSurface {
    X_UP,
    X_SAME,
    X_DOWN,
    Y_UP,
    Y_SAME,
    Y_DOWN,
    X_UP_Y_UP,
    X_UP_Y_DOWN,
    X_DOWN_Y_UP,
    X_DOWN_Y_DOWN,
    Z_UP,
    Z_SAME,
    Z_DOWN
  };
  enum class PortationType { ENTER, EXIT, PERIODIC };

public:
  MultipleSubworld(const G4String name, G4VPhysicalVolume *volume,
                   G4ThreeVector &vec, const G4int verbose,
                   FacetStore *facetStore = nullptr);
  ~MultipleSubworld();
  virtual void DoPortation(const G4Step *step);
  // Setter
  void SetGrid(const G4int nX, const G4int nY, const G4int verbose = 3);
  void SetGrid(SubworldGrid<MultipleSubworld> *grid);
  void AddSubworldToGrid(const G4int x, const G4int y,
                         MultipleSubworld *subworld);
  void SetAsPortal() { fIsPortal = true; }
  void SetOtherPortal(MultipleSubworld *otherPortal);
  // Getter
  SubworldGrid<MultipleSubworld> *GetSubworldGrid() const {
    return fSubworldGrid;
  }
  // Check
  inline G4bool IsPortal() const { return fIsPortal; }

  void SetSubwordlEdge(G4double edgeX, G4double edgeY, G4double edgeZ);

  FacetStore *GetFacetStore() { return fFacetStore; };

private:
  void DoPeriodicPortation(const G4Step *step, const SingleSurface);
  void EnterPortal(const G4Step *step, const SingleSurface);
  void ExitPortal(const G4Step *step, const SingleSurface);

  SingleSurface GetNearestSurface(const G4Step *step);
  PortationType GetPortationType(const SingleSurface);

  void DoPeriodicTransform(G4ThreeVector &vec, SingleSurface);
  void TransformSubworldToPortal(G4ThreeVector &vec);
  void TransformPortalToSubworld(G4ThreeVector &vec);
  G4double TransformZBetweenPortals(const G4double val);

  void LoggCurrentStatus();

private:
  Logger fLogger;
  MultipleSubworld *fPortal; // TO DO: replace this or delete it!! In transform
                             // function, example values must be set
  G4double fEdgeX, fEdgeY, fEdgeZ;
  G4VPhysicalVolume *fTrigger;
  G4bool fIsPortal; // True if it is the portal, false if it is the periodic
                    // subworld
  SubworldGrid<MultipleSubworld> *fSubworldGrid;
  // order of Grid such that (fCurrentNX, fCurrentNy) = (0,0)
  // has the lowest local (x,y) coordinate in the portal
  // Grid will only be initialized if called
  FacetStore *fFacetStore;
};
} // namespace Surface
#endif // MULTIPLESUBWORLD_HH
