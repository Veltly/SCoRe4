/**
 * @brief Definition of standard portal
 * @author C.Gruener
 * @date 2024-06-11
 * @file MultipleSubworld.hh
 */
#ifndef SRC_PORTAL_INCLUDE_MULTIPLESUBWORLD_HH
#define SRC_PORTAL_INCLUDE_MULTIPLESUBWORLD_HH

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/SubworldGrid.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
#include "VPortal.hh"

namespace Surface {
/**
 * @brief Class defines the standard portal with a multiple subworlds
 * in a grid-like structure
 */
class MultipleSubworld : public VPortal {
  /**
   * @brief enum for all directions a particle can take to exit a volume
   */
  enum class Direction {
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

  /**
   * @brief definition of portation type
   * @details Enter: enters portal volume
   * Exit: exits portal volume
   * Periodic: enters another subworld (only possible if already in portal)
   */
  enum class PortationType { ENTER, EXIT, PERIODIC };

 public:
  MultipleSubworld(const G4String &name, G4VPhysicalVolume *volume,
                   const G4ThreeVector &vec, VerboseLevel verbose = VerboseLevel::Default,
                   FacetStore *facetStore = nullptr);

  MultipleSubworld(const G4String &name, G4VPhysicalVolume *volume,
                   G4Transform3D transform, VerboseLevel verbose = VerboseLevel::Default,
                   FacetStore *facetStore = nullptr);

  ~MultipleSubworld();

  void DoPortation(G4Step *step) override;

  // Setter
  void SetGrid(G4int nX, G4int nY, VerboseLevel verbose = VerboseLevel::Default);

  void SetGrid(SubworldGrid<MultipleSubworld> *grid);

  void AddSubworldToGrid(G4int x, G4int y,
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

  void SetSubworldEdge(G4double edgeX, G4double edgeY,
                       G4double edgeZ);

  inline FacetStore *GetFacetStore() const { return fFacetStore; }

 private:
  void DoPeriodicPortation(G4Step *step, Direction);

  void EnterPortal(G4Step *step);

  void ExitPortal(G4Step *step);

  Direction GetNearestSurface(const G4Step *step);

  PortationType GetPortationType(Direction);

  void DoPeriodicTransform(G4ThreeVector &vec, Direction);

  void TransformSubworldToPortal(G4ThreeVector &vec);

  void TransformPortalToSubworld(G4ThreeVector &vec);

  G4double TransformZBetweenPortals(G4double val);

  std::string CurrentStatusString() const;

 private:
  //  Logger fLogger;
  MultipleSubworld *fPortal{};  // TO DO: replace this or delete it!! In transform
                              // function, example values must be set
  G4double fEdgeX{}, fEdgeY{}, fEdgeZ{};
  G4bool fIsPortal;  // True if it is the portal, false if it is the periodic
                     // subworld
  SubworldGrid<MultipleSubworld> *fSubworldGrid;
  // order of Grid such that (fCurrentNX, fCurrentNy) = (0,0)
  // has the lowest local (x,y) coordinate in the portal
  // Grid will only be initialized if called
  FacetStore *fFacetStore;
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_MULTIPLESUBWORLD_HH
