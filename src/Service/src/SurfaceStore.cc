/**
 * @brief Implementation of SurfaceStore
 * @author C.Gruener
 * @date 2024-05-15
 * @file SurfaceStore.cc
 */

#include "Service/include/SurfaceStore.hh"

// for singleton init to null
Surface::SurfaceStore* Surface::SurfaceStore::fStore = nullptr;

Surface::SurfaceStore::SurfaceStore() : fLogger("SurfaceStore") {}

Surface::SurfaceStore& Surface::SurfaceStore::GetInstance() {
  if (fStore == nullptr) {
    fStore = new SurfaceStore();
  }
  return *fStore;
}

Surface::FacetStore* Surface::SurfaceStore::Surface(
    const G4String& surfaceName) {
  for (auto* surface : fSurfaceStore) {
    if (surface->GetStoreName() == surfaceName) {
      return surface;
    }
  }
  fLogger.WriteWarning("No FacetStore with name: " + surfaceName + " found!");
  return nullptr;
}

void Surface::SurfaceStore::AddSurface(FacetStore* store) {
  if (store == nullptr) {
    fLogger.WriteWarning("No FacetStore added to SurfaceStore, ptr is nullptr");
    return;
  }
  fSurfaceStore.push_back(store);
}