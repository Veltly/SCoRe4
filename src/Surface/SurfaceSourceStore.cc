/**
* @brief Implementation of SurfaceSourceStore
* @author C.Gruener
* @date 2025-10-25
* @file SurfaceSourceStore.cc
*/

#include "SurfaceSourceStore.hh"

namespace Surface {

  SurfaceSourceStore::SurfaceSourceStore() {
    f_volume.clear();
    f_rotation.clear();
    f_position.clear();
  }

  void SurfaceSourceStore::add(G4RotationMatrix *matrix,
                               const G4ThreeVector &position,
                               G4LogicalVolume *volume) {
    f_rotation.push_back(matrix);
    f_position.push_back(position);
    f_volume.push_back(volume);
  }

  G4String SurfaceSourceStore::information() const {
    std::stringstream stream;
    stream << "\n";
    stream << "**************************************************\n";
    stream << "*****  Information of Surface-Source-Store   *****\n";
    stream << "**************************************************\n";
    stream << "Name: " << f_name << "\n";
    stream << "Elements in store: " << f_volume.size() << "\n";
    stream << "**************************************************\n";
    stream << "**************************************************\n";
    return stream.str();
  }
  }  // namespace Surface