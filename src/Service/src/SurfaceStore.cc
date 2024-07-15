// Copyright [2024] C.Gruener
// Date: 24-07-15
// File: SurfaceStore saves all FacetStores at one point

#include "Service/include/SurfaceStore.hh"

// for singleton init to null
Surface::SurfaceStore* Surface::SurfaceStore::fStore = nullptr;

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
  return nullptr;
}
