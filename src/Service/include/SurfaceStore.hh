// Copyright [2024] C.Gruener
// Date: 24-07-15
// File: SurfaceStore saves all FacetStores at one point

#ifndef SRC_SERVICE_INCLUDE_SURFACESTORE_HH_
#define SRC_SERVICE_INCLUDE_SURFACESTORE_HH_

#include <vector>

#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
namespace Surface {

class SurfaceStore {
 public:
  static SurfaceStore &GetInstance();

  SurfaceStore(SurfaceStore &) = delete;
  void operator=(const SurfaceStore &) = delete;

  ~SurfaceStore() {
    for (const auto *surface : fSurfaceStore) {
      delete surface;
      surface = nullptr;
    }
  }

  void AddSurface(FacetStore *store);
  FacetStore *Surface(const G4String &surfaceName);

 private:
  SurfaceStore();

 private:
  static SurfaceStore *fStore;
  Logger fLogger;
  std::vector<Surface::FacetStore *> fSurfaceStore;
};
}  // namespace Surface
#endif  //  SRC_SERVICE_INCLUDE_SURFACESTORE_HH_
