/**
 * @brief Definition of PortalControl class
 * @author C.Gruener
 * @date 2024-05-25
 * @file PortalControl.hh
 */

#ifndef SRC_PORTAL_INCLUDE_PORTALCONTROL_HH
#define SRC_PORTAL_INCLUDE_PORTALCONTROL_HH

#include <G4VPhysicalVolume.hh>

#include "G4UserSteppingAction.hh"
#include "Portal/include/PortalStore.hh"
#include "Service/include/Logger.hh"
namespace Surface {
/**
 * @brief Class controls the implemented portal during simulation
 */
class PortalControl {
 public:
  explicit PortalControl(VerboseLevel verboseLvl = VerboseLevel::Default);
  void DoStep(G4Step *step);
  void DoStep(const G4Step *step);
  void DoPortation(G4Step *step, const G4VPhysicalVolume *volume);
  G4bool EnterPortalCheck(const G4Step *step);
  void SetVerbose(VerboseLevel verboseLvl);
  G4bool IsVolumeInsidePortal(const G4VPhysicalVolume *volume) const;
  void UsePortal(G4Step *step);

 private:
  PortalStore &fPortalStore;
  Logger fLogger;
  G4bool fJustPorted{};
  G4bool fInSubworld{};
  G4StepPoint fRecentStepPoint;
  G4bool fInPortal{};
};
}  // namespace Surface
#endif  // SRC_PORTAL_INCLUDE_PORTALCONTROL_HH
