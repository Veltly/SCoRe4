// Copyright [2024] C.Gruener
// Date: 24-06-11
// File: MultipleSubworlds

#ifndef SRC_PORTAL_INCLUDE_MULTIPLESUBWORLD_HH_
#define SRC_PORTAL_INCLUDE_MULTIPLESUBWORLD_HH_

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
#include "VPortal.hh"

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
                   const G4ThreeVector &vec, const G4int verbose,
                   FacetStore *facetStore = nullptr);

  MultipleSubworld(const G4String name, G4VPhysicalVolume *volume,
                   const G4Transform3D transform, const G4int verbose,
                   FacetStore *facetStore = nullptr);

  ~MultipleSubworld();

  virtual void DoPortation(G4Step *step);

  // Setter
  void SetGrid(const G4int nX, const G4int nY, const G4int verbose = 3);

  void SetGrid(SubworldGrid<MultipleSubworld> *grid);

  void AddSubworldToGrid(const G4int x, const G4int y,
                         MultipleSubworld *subworld);

  void SetAsPortal() { fIsPortal = true; }

  void SetOtherPortal(MultipleSubworld *otherPortal);

  void SetFacetStore(FacetStore *);

  // Getter
  SubworldGrid<MultipleSubworld> *GetSubworldGrid() const {
    return fSubworldGrid;
  }

  // Check
  inline G4bool IsPortal() const { return fIsPortal; }

  void SetSubworldEdge(const G4double edgeX, const G4double edgeY,
                       const G4double edgeZ);

  inline FacetStore *GetFacetStore() const { return fFacetStore; }

 private:
  void DoPeriodicPortation(G4Step *step, const SingleSurface);

  void EnterPortal(G4Step *step);

  void ExitPortal(G4Step *step);

  SingleSurface GetNearestSurface(const G4Step *step);

  PortationType GetPortationType(const SingleSurface);

  void DoPeriodicTransform(G4ThreeVector &vec, const SingleSurface);

  void TransformSubworldToPortal(G4ThreeVector &vec);

  void TransformPortalToSubworld(G4ThreeVector &vec);

  G4double TransformZBetweenPortals(const G4double val);

  void LogCurrentStatus();

 private:
  Logger fLogger;
  MultipleSubworld *fPortal;  // TO DO: replace this or delete it!! In transform
                              // function, example values must be set
  G4double fEdgeX, fEdgeY, fEdgeZ;
  G4bool fIsPortal;  // True if it is the portal, false if it is the periodic
                     // subworld
  SubworldGrid<MultipleSubworld> *fSubworldGrid;
  // order of Grid such that (fCurrentNX, fCurrentNy) = (0,0)
  // has the lowest local (x,y) coordinate in the portal
  // Grid will only be initialized if called
  FacetStore *fFacetStore;
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_MULTIPLESUBWORLD_HH_
