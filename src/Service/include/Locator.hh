// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#ifndef SRC_SERVICE_INCLUDE_LOCATOR_HH_
#define SRC_SERVICE_INCLUDE_LOCATOR_HH_

#include "Portal/include/PortalStore.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/FacetStore.hh"

namespace Surface {

///
/// The Locator is used to have access to registered services from anywhere in
/// the code
///

class Locator {
 public:
  ///
  /// Returns reference to FacetStore
  ///
  static Surface::FacetStore &GetFacetStore() { return fFacetStore; }
  ///
  /// Returns reference to Logger
  ///
  static Surface::Logger &GetLogger() { return fLogger; }
  ///
  /// Returns reference to PortalStore
  ///
  static Surface::PortalStore &GetPortalStore() { return fPortalStore; }

 private:
  static Surface::FacetStore fFacetStore;
  static Surface::Logger fLogger;
  static Surface::PortalStore fPortalStore;
};
}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_LOCATOR_HH_
