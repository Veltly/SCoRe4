/**
 * @brief Handles the portal interface
 * @file VPortal.hh
 * @author C.Gruener
 * @date 24-05-24
 */

#ifndef SRC_PORTAL_INCLUDE_VPORTAL_HH_
#define SRC_PORTAL_INCLUDE_VPORTAL_HH_

#include <cstdlib>

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "Service/include/Logger.hh"

namespace Surface {

/**
 * @brief Enumerates the different types of portals
 */
enum class PortalType { SimplePortal, PeriodicPortal, MultipleSubworld };

/**
 * @brief The class is the interface for a portal
 */
class VPortal {
 public:
  virtual void DoPortation(G4Step *) = 0;  /// Does the portation of a tracked particle

  VPortal(const G4String &name, G4VPhysicalVolume *, PortalType,
          G4int verboseLvl);
  VPortal(const G4String &name, G4VPhysicalVolume *, PortalType,
          G4int verboseLvl, const G4ThreeVector &globalCoord);

  // Getter
  inline G4VPhysicalVolume *GetVolume() const { return fVolume; }
  inline G4String GetName() const { return fName; }
  inline PortalType GetPortalType() const { return fPortalType; }
  inline G4VPhysicalVolume *GetTrigger() const { return fTrigger; }

  // Setter
  void SetGlobalCoord(G4ThreeVector vec);
  void SetVerbose(G4int verboseLvl);
  void SetTrigger(G4VPhysicalVolume *volume); ///set the trigger of a portal (when trigger is entered, the portal is used

 protected:
  G4ThreeVector GetLocalCoordSystem() const;

  void TransformToLocalCoordinate(G4ThreeVector &vec);
  void TransformToGlobalCoordinate(G4ThreeVector &vec);
  void UpdatePosition(G4Step *step, const G4ThreeVector &newPosition);
  void UpdatePositionMomentum(G4Step *step, const G4ThreeVector &newPosition,
                              const G4ThreeVector &newDirection);

  Logger fLogger;

 private:
  const G4String fName;
  G4VPhysicalVolume *fVolume;
  const PortalType fPortalType;
  G4ThreeVector fGlobalCoord;
  G4bool fGlobalCoordSet;
  G4VPhysicalVolume *fTrigger;
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_VPORTAL_HH_
