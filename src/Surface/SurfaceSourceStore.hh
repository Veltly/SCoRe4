/**
 * @brief Definition of class SurfaceSourceStore
 * @author C.Gruener
 * @date 2025-10-25
 * @file SurfaceSourceStore.hh
 */

#ifndef SURFACE_SURFACESOURCESTORE_HH
#define SURFACE_SURFACESOURCESTORE_HH

#include "G4String.hh"
#include "Service/include/Logger.hh"
#include "G4LogicalVolume.hh"
#include <vector>

namespace Surface {
/**
 * @brief Singleton class holding all surfaces that are needed for SurfaceSource
 * @details the class can only be called by class Surface and class SurfacePlacement
 */
class SurfaceSourceStore{
 private:
  SurfaceSourceStore();

 public:
  SurfaceSourceStore(const SurfaceSourceStore&) = delete;
  SurfaceSourceStore& operator=(const SurfaceSourceStore&) = delete;

 friend class SurfacePlacement;
 friend class Source;

  static SurfaceSourceStore& getInstance(){
    static SurfaceSourceStore instance;
    return instance;
  }

  void add(G4RotationMatrix *matrix, const G4ThreeVector& position, G4LogicalVolume* volume);

  inline G4RotationMatrix* get_rotation(size_t idx) const {
    return f_rotation.at(idx);
  }

  inline G4ThreeVector get_position(size_t idx) const {
    return f_position.at(idx);
  }

  inline G4LogicalVolume* get_volume(size_t idx) const {
    return f_volume.at(idx);
  }

  inline size_t size() const {return f_volume.size();}

  G4String information() const;

 private:
  std::vector<G4LogicalVolume*> f_volume;
  std::vector<G4RotationMatrix*> f_rotation;
  std::vector<G4ThreeVector> f_position;
  G4String f_name{"SurfaceSourceStore"};
  Logger f_logger{f_name, VerboseLevel::Default};
};

}  // namespace Surface

#endif  // SURFACE_SURFACESOURCESTORE_HH
