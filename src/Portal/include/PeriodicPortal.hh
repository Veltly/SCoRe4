/**
 * @brief Periodic portal reused a small simulation domain repeatedly to represent a larger volume
 * @file PeriodicPortal.hh
 * @auhtor C.Gruener
 * @date 24-06-04
 */
#ifndef SRC_PORTAL_INCLUDE_PERIODICPORTAL_HH_
#define SRC_PORTAL_INCLUDE_PERIODICPORTAL_HH_

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/VPortal.hh"
#include "Service/include/Logger.hh"

namespace Surface {
/**
 * @brief Periodic portal reuses a small simulation domain repeatedly to represent a larger volume
 * @details The entrance portal is represented by small er simulation domain which is used repeatedly
 * Therefor the entrance is an integer multiple of the small simulation domain which is represented by
 * a 2D grid structure where each gridpoint is represented by the same small simulation domain.
 * Therefor, the class tracks the movement of the particle by storing the current particles gridpoint.
 * If the particle exits one simulation volume which is NOT ON the edge of the grid, it exits the small
 * simulation volume on one side and enters it again on the other end.
 * If the particle exits one simulation volume which is ON the edge of the grid with no neighboring
 * gridpoint in the exit direction, the particle exits the portal.
 * The grid has indices from (0,0) -> (Nx-1,Ny-1)
 */
class PeriodicPortal : public VPortal {
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
  PeriodicPortal(const G4String &name, G4VPhysicalVolume *volume,
                 const G4ThreeVector &vec, VerboseLevel verbose = VerboseLevel::Default);
/**
 * @brief Performs the portation
 * @param step
 */
  void DoPortation(G4Step *step) override;

  // Setter
  void SetGrid(int nX, int nY); ///Set the dimension of the grid
  inline void SetAsPortal() { fIsPortal = true; } ///Set if this is not the periodic portal
  void SetOtherPortal(PeriodicPortal *otherPortal); ///Set periodic portal

  // Getter
  inline G4int GetMaxNX() const { return fMaxNX; }
  inline G4int GetMaxNY() const { return fMaxNY; }
  inline G4int GetCurrentNX() const { return fCurrentNX; }
  inline G4int GetCurrentNY() const { return fCurrentNY; }

  // Check
  inline G4bool IsPortal() const { return fIsPortal; }

 private:
  void DoPeriodicPortation(G4Step *step, SingleSurface);
  void EnterPortal(G4Step *step);
  void ExitPortal(G4Step *step);
/**
 * @brief Returns closest surface of periodic portal volume
 * @param step
 * @return
 */
  SingleSurface GetNearestSurface(const G4Step *step);
  /**
   * @brief Returns the needed portation type based on the closest surface
   * @return
   */
  PortationType GetPortationType(SingleSurface) const;

  void DoPeriodicTransform(G4ThreeVector &vec, SingleSurface);
  void TransformSubworldToPortal(G4ThreeVector &vec);
  void TransformPortalToSubworld(G4ThreeVector &vec);
  G4double TransformZBetweenPortals(G4double val);

 private:
  PeriodicPortal *fOtherPortal{}; /// handle to the periodic subworld
  G4bool fIsPortal;  /// True if it is the portal, false if it is the periodic subworld
  G4int fMaxNX; /// Grid size in  X direction
  G4int fMaxNY; /// Grid size in Y direction
  G4int fCurrentNX; /// Current grid location of tracked particle in X dimension
  G4int fCurrentNY; /// Current grid location of tracked particle in Y dimension
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_PERIODICPORTAL_HH_
