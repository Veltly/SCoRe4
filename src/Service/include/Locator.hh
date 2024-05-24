//
//
//
//
//	author:C.Gruener
//
//

#ifndef SURFACE_LOCATOR
#define SURFACE_LOCATOR

#include "../../Portal/include/PortalStore.hh"
#include "../../SurfaceGenerator/include/FacetStore.hh"
#include "Logger.hh"
namespace Surface {

///
/// The Locator is used to have access to registered services from anywehre in
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
} // namespace Surface
#endif
