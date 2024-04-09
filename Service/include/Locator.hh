//
//
//
//
//	author:C.Gruener
//
//

// Locator is used to have access to the service from anywhere in the code
// Service has to be registered once

#ifndef SURFACE_LOCATOR
#define SURFACE_LOCATOR

#include "../../SurfaceGenerator/include/FacetStore.hh"
#include "Logger.hh"
namespace Surface {
class Locator {
public:
  static Surface::FacetStore &GetFacetStore() { return fFacetStore; }
  static Surface::Logger &GetLogger() { return fLogger; }

private:
  static Surface::FacetStore fFacetStore;
  static Surface::Logger fLogger;
};
} // namespace Surface
#endif
