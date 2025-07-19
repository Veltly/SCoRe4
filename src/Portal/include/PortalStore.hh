/**
 * @brief Stores a handle to all portals
 * @file PortalStore.hh
 * @author C.Gruener
 * @date 24-05-24
 */
#ifndef SRC_PORTAL_INCLUDE_PORTALSTORE_HH_
#define SRC_PORTAL_INCLUDE_PORTALSTORE_HH_

#include <vector>

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/VPortal.hh"

namespace Surface {
/**
 * @brief Stores a handle to all used portals
 * @details Stores a handle to all used portals/triggers. Portals/Triggers can be found based
 * on it G4PhysicalVolume oder G4LogicalVolume or name
 */
class PortalStore : public std::vector<VPortal *> {
 public:
  G4bool IsLogPortal(const G4LogicalVolume *volume) const;
  G4bool IsTrigger(const G4VPhysicalVolume *volume) const;

  G4int FindPortal(const G4VPhysicalVolume *volume) const;
  G4int FindPortalId(const G4String &name) const;
  G4int FindLogPortal(const G4LogicalVolume *volume) const;
  G4int FindTrigger(const G4VPhysicalVolume *volume) const;

  VPortal *GetPortal(const G4VPhysicalVolume *volume) const;
};
}  // namespace Surface

#endif  // SRC_PORTAL_INCLUDE_PORTALSTORE_HH_
