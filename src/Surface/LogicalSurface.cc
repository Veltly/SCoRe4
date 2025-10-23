/**
 * @brief Implementation of LogicalSurface.hh
 * @author C.Gruener
 * @date 2025-10-23
 * @file LogicalSurface.cc
 */

#include <utility>

#include "LogicalSurface.hh"
#include "G4GDMLParser.hh"
#include "G4SolidStore.hh"
#include "G4PVPlacement.hh"
#include "G4Exception.hh"

namespace Surface {

LogicalSurface::LogicalSurface(G4String name, G4String  gdml_filename,
                               G4int nx, G4int ny, G4Material* material,
                               G4Material* envelope_material,
                               VerboseLevel verbose_lvl)
    : f_name(std::move(name)), f_gdml_filename(std::move(gdml_filename)),
      f_nx(nx), f_ny(ny),
      f_material(material), f_envelope_material(envelope_material),
      f_logger("LogicalSurfaceVolume", verbose_lvl) {
  load_gdml();
  place_surface_element_inside_volume();
  f_logger.WriteInfo([this]{return this->solid_information();});
}

void LogicalSurface::load_gdml() {
  G4GDMLParser parser;
  const G4bool validate_gdml{false};
  //validation should be done by the python module generating the gdml file
  parser.Read(f_gdml_filename, validate_gdml);
  const G4SolidStore *store = G4SolidStore::GetInstance();
  G4VSolid *importedSolid = store->back();
  if(!importedSolid){
    G4Exception("DetectorConstruction::Construct()",
                "",FatalException,"Failed to retrieve solid from GDML!");
  }
  f_logger.WriteDebugInfo("Solid loaded from GDML file " + f_gdml_filename);
  f_surface_element = dynamic_cast<G4TessellatedSolid*>(importedSolid);
}

std::string LogicalSurface::solid_information() const {
  std::stringstream stream;
  stream << "\n";
  stream << "**************************************************\n";
  stream << "*****  Information of LogicalSurface Object  *****\n";
  stream << "**************************************************\n";
  stream << "* LogicalSurface Object: " << f_name << "\n";
  stream << "* Solid name: " << f_surface_element->GetName() << "\n";
  stream << "* Memory facets: " << f_surface_element->AllocatedMemoryWithoutVoxels()/1e6 << " MB \n";
  stream << "* Memory total : " << f_surface_element->AllocatedMemory()/1e6 << "MB \n";
  stream << "* Number of facets: " << f_surface_element->GetNumberOfFacets() << "\n";
  stream << "* Max extension x: " << f_surface_element->GetMaxXExtent() * f_nx << "\n";
  stream << "* Max extension y: " << f_surface_element->GetMaxYExtent() * f_ny << "\n";
  //must add height information
  stream << "**************************************************\n";
  stream << "**************************************************\n";
  return stream.str();
}
void LogicalSurface::place_surface_element_inside_volume() {
  double extend_x = f_surface_element->GetMaxXExtent();
  double extend_y = f_surface_element->GetMaxYExtent();
  double extend_z = f_surface_element->GetMaxXExtent();

  int nx{f_nx};
  int ny{f_ny};

  G4double envelope_size_x = extend_x * nx;
  G4double envelope_size_y = extend_y * ny;
  G4double envelope_size_z = extend_z;


  auto *envelope_solid = new G4Box(f_name + "_SD",envelope_size_x,
                                   envelope_size_y, envelope_size_z);

  f_logical_envelope = new G4LogicalVolume(envelope_solid,
                                         f_envelope_material,
                                         f_name);

  auto *logical_surface_element = new G4LogicalVolume(f_surface_element,
                                                      f_material,
                                                      f_name + "_surface_LV");
//place surface element inside envelope
  for(int ix=0; ix < nx; ix++){
    for(int iy=0; iy < ny; iy++){
      G4ThreeVector surfacePlacement{-envelope_size_x + extend_x + 2 * ix * extend_x,
                                     -envelope_size_y + extend_y + 2 * iy * extend_y,
                                     -envelope_size_z};
      auto name = "LogicalSurface_"
                  + std::to_string(ix) + "_"
                  + std::to_string(iy);

      new G4PVPlacement(nullptr,surfacePlacement,logical_surface_element,
                        name,f_logical_envelope,false,0,true);
    }
  }
}

G4LogicalVolume *LogicalSurface::get_logical_handle() {
  if (f_logical_envelope) {
    return f_logical_envelope;
  }
  G4Exception("LogicalSurface::get_logical_handle()",
              "", FatalException,
              "Handle to logical_envelope_volume is nullptr!");
  return nullptr;
}

}  // namespace LogicalSurface