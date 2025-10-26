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
#include "G4TriangularFacet.hh"

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



  G4LogicalVolume* get_logical_handle();

  void sample_point(G4ThreeVector &point, G4ThreeVector &direction);

  G4double surface_area() const;

  void force_probability_generation();

  void show_information() const;
  void show_probability_information() const;
  void show_placed_elements_information()const;

 private:
  void load_gdml();
  void place_surface_element_inside_volume();

  static G4bool facet_above_height(G4TriangularFacet * facet) ;

  void fill_facet_store();
  void generate_probability();

  size_t random_select_placed_element() const;
  size_t random_select_facet() const;

  G4String information() const;
  G4String probability_information() const;
  G4String placed_elements_information() const;



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
  std::vector<G4TriangularFacet*> f_facets;
  std::vector<G4double> f_probability;
  G4bool f_probability_generated{false};

};

}  // namespace Surface

#endif  // LogicalSurface_LogicalSurface_HH
