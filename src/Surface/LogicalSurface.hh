/**
 * @brief Definition of LogicalSurface class
 * @author C.Gruener
 * @date 2025-10-23
 * @file LogicalSurface.hh
 */
#ifndef LogicalSurface_LogicalSurface_HH
#define LogicalSurface_LogicalSurface_HH

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4String.hh"
#include "G4TessellatedSolid.hh"
#include "Service/include/Logger.hh"

namespace Surface {
/**
 * @brief LogicalSurface class for placement of LogicalSurface
 * @details The class handles loading of LogicalSurface from GDML file and placing it
 * multiple times at a defined position.
 */
class LogicalSurface {
 public:
  LogicalSurface(G4String name, G4String gdml_filename, G4int nx, G4int ny,
                 G4Material* material, G4Material* envelope_material,
                 VerboseLevel verbose_lvl = VerboseLevel::Default);

  G4int nx_extension() const {return f_nx;};
  inline void nx_extension(G4int nx){f_nx = nx;}

  G4int ny_extension() const {return f_ny;};
  inline void ny_extension(G4int ny){f_ny = ny;}

  std::string solid_information() const;

  G4LogicalVolume* get_logical_handle();

 private:
  void load_gdml();
  void place_surface_element_inside_volume();

 private:
  G4String f_name;
  G4String f_gdml_filename;
  G4int f_nx;
  G4int f_ny;
  G4Material *f_material;
  G4Material *f_envelope_material;
  Logger f_logger;
  G4TessellatedSolid *f_surface_element{nullptr};
  G4LogicalVolume *f_logical_envelope{nullptr};
};

}  // namespace LogicalSurface

#endif  // LogicalSurface_LogicalSurface_HH
