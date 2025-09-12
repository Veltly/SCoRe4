/**
 * @brief Class SurfaceStore holds handles to FacetStore
 * @author C.Gruener
 * @date 2024-07-15
 * @file SurfaceStore.hh
 */

#ifndef SRC_SERVICE_INCLUDE_SURFACESTORE_HH
#define SRC_SERVICE_INCLUDE_SURFACESTORE_HH

#include <vector>

#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
namespace Surface {
/**
 * @brief The class SurfaceStore is a singleton class and holds handles to all FacetStores
 * @details This is useful if multiple FacetStores are used
 */
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
#endif  //  SRC_SERVICE_INCLUDE_SURFACESTORE_HH
