/**
 * @brief Stores a handle to all portals
 * @file PortalStore.hh
 * @author C.Gruener
 * @date 24-05-24
 */
#ifndef SRC_PORTAL_INCLUDE_PORTALSTORE_HH
#define SRC_PORTAL_INCLUDE_PORTALSTORE_HH

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
  /**
   * @brief Checks if volume is logical portal
   * @param volume to check
   * @return true if volume is portal
   */
  G4bool IsLogPortal(const G4LogicalVolume *volume) const;
  /**
   * @brief Checks if volume is a trigger
   * @param volume to check
   * @return true if volume is a trigger
   */
  G4bool IsTrigger(const G4VPhysicalVolume *volume) const;

  /**
   * @brief returns index of portal based on physical volume
   * @param volume physical portal to find
   * @return index of portal, else -1
   */
  G4int FindPortal(const G4VPhysicalVolume *volume) const;
  /**
   * @brief returns index of portal based on name
   * @param name portal to find
   * @return index of portal, else -1
   */
  G4int FindPortalId(const G4String &name) const;
  /**
   * @brief returns index of portal based on logical volume
   * @param volume logical volume portal to find
   * @return index of portal, else -1
   */
  G4int FindLogPortal(const G4LogicalVolume *volume) const;
  /**
   * @brief find index of portal with physical trigger volume
   * @param volume physical trigger volume
   * @return index of portal, else -1
   */
  G4int FindTrigger(const G4VPhysicalVolume *volume) const;

  /**
   * @brief returns pointer to portal based on physical volume
   * @param volume physical volume
   * @return pointer to portal, nullptr if does not exist
   */
  VPortal *GetPortal(const G4VPhysicalVolume *volume) const;
};
}  // namespace Surface

#endif  // SRC_PORTAL_INCLUDE_PORTALSTORE_HH
