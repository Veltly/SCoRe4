/**
 * @brief Handle for singleton classes
 * @author C.Gruener
 * @date 2023-06-01
 * @file Locator.hh
 */

#ifndef SRC_SERVICE_INCLUDE_LOCATOR_HH
#define SRC_SERVICE_INCLUDE_LOCATOR_HH

#include "Portal/include/PortalStore.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/FacetStore.hh"

namespace Surface {
/**
 * @brief The locator class is used to have access to registered services from anywhere in the code
 */
class Locator {
 public:
  /**
   * @return Reference to FacetStore
   */
  static Surface::FacetStore &GetFacetStore() { return fFacetStore; }
  /**
   * @return Reference to PortalStore
   */
  static Surface::PortalStore &GetPortalStore() { return fPortalStore; }

 private:
  static Surface::FacetStore fFacetStore;
  static Surface::Logger fLogger;
  static Surface::PortalStore fPortalStore;
};
}  // namespace Surface
#endif // SRC_SERVICE_INCLUDE_LOCATOR_HH