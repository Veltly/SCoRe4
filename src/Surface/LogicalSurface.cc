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
#include "Randomize.hh"
#include <numeric>

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
  f_logger.WriteInfo([this]{return this->information();});
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

void LogicalSurface::place_surface_element_inside_volume() {
  const double extend_x = f_surface_element->GetMaxXExtent();
  const double extend_y = f_surface_element->GetMaxYExtent();
  const double extend_z = f_surface_element->GetMaxXExtent();

  const int nx{f_nx};
  const int ny{f_ny};

  const G4double envelope_size_x = extend_x * nx;
  const G4double envelope_size_y = extend_y * ny;
  const G4double envelope_size_z = extend_z;
  f_shift_to_zero = envelope_size_z;

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

G4bool LogicalSurface::facet_above_height(G4TriangularFacet * facet) {
  const G4double height = 0.0; // edge of surface/body is shifted to zero
  for(G4int idx = 0; idx < 3; idx++){
    const auto vertex = facet->GetVertex(idx);
    if(vertex.getZ() < height){
      return false;
    }
  }
  return true;
}

G4bool LogicalSurface::facet_part_of_surface(G4TriangularFacet *facet){
  for(G4int idx = 0; idx < 3; idx++){
    const auto vertex = facet->GetVertex(idx);
    if(vertex.getZ() == 0.){
      return false;
    }
  }
  return true;
}

size_t LogicalSurface::random_select_facet() const {
  const G4double random = G4UniformRand();
  const size_t size = f_probability.size();
  for (size_t idx = 0; idx < size; idx++) {
    if (random <= f_probability[idx]) {
      return idx;
    }
  }
  exit(EXIT_FAILURE);  // should never happen
}

size_t LogicalSurface::random_select_placed_element() const{
  const auto idx = static_cast<size_t>(f_nx * f_ny * G4UniformRand());
  return idx;
}

void LogicalSurface::sample_point(G4ThreeVector &point,
                                  G4ThreeVector &direction) {
  if(!f_probability_generated){
    generate_probability();
  }
  const auto element_idx = random_select_placed_element();
  auto *daughter = f_logical_envelope->GetDaughter(static_cast<G4int>(element_idx));
  G4ThreeVector element_position = daughter->GetTranslation();
  const auto facet_idx = random_select_facet();
  const auto *facet = f_surface_element->GetFacet(static_cast<G4int>(facet_idx));
  const auto point_on_facet = facet->GetPointOnFace();
  point = point_on_facet + element_position;
  direction = facet->GetSurfaceNormal();
  f_logger.WriteDebugInfo("Sampled element idx: " + std::to_string(element_idx));
  f_logger.WriteDebugInfo("Sampled   facet idx: " + std::to_string(facet_idx));
}
G4double LogicalSurface::surface_area() const {
  G4double area{0};
  for (auto *facet:f_facets) {
    area += facet->GetArea();
  }
  return area;
}

void LogicalSurface::fill_facet_store() {
  f_logger.WriteDetailInfo("Filling facet store");
  f_facets.clear();
  size_t number_of_facets = f_surface_element->GetNumberOfFacets();
  for(size_t idx = 0; idx < number_of_facets; idx++){
    auto *facet = dynamic_cast<G4TriangularFacet*>(f_surface_element->GetFacet(static_cast<G4int>(idx)));
    if(facet_part_of_surface(facet)){
      f_facets.push_back(facet);
    }
  }
  f_logger.WriteDebugInfo("Filled facet store with " + std::to_string(f_facets.size()) + " facets");
}
void LogicalSurface::generate_probability() {
  f_logger.WriteDetailInfo("Generating probability vector");
  fill_facet_store();
  const auto size = f_facets.size();
  std::vector<G4double> areas;
  areas.reserve(size);
  for (auto *facet:f_facets) {
    areas.push_back(facet->GetArea());
  }
  f_probability.clear();
  f_probability.reserve(size);
  const G4double total_area = std::accumulate(areas.begin(), areas.end(), 0.);
  G4double cumulative_sum{0};
  for (auto area : areas) {
    cumulative_sum += area;
    f_probability.push_back(cumulative_sum / total_area);
  }
  f_probability_generated = true;
}


G4String LogicalSurface::information() const {
  std::stringstream stream;
  stream << "\n";
  stream << "**************************************************\n";
  stream << "*****  Information of LogicalSurface Object  *****\n";
  stream << "**************************************************\n";
  stream << "* LogicalSurface Object: " << f_name << "\n";
  stream << "* GDML filename: " << f_gdml_filename << "\n";
  stream << "* Solid name: " << f_surface_element->GetName() << "\n";
  stream << "* Memory facets: " << f_surface_element->AllocatedMemoryWithoutVoxels()/1e6 << " MB \n";
  stream << "* Memory total : " << f_surface_element->AllocatedMemory()/1e6 << "MB \n";
  stream << "* Number of facets: " << f_surface_element->GetNumberOfFacets() << "\n";
  stream << "* Max extension x: " << f_surface_element->GetMaxXExtent() * f_nx << "\n";
  stream << "* Max extension y: " << f_surface_element->GetMaxYExtent() * f_ny << "\n";
  //must add height information
  stream << "* Placed elements: nx=" << f_nx << "ny=" << f_ny << "total=" << f_nx*f_ny << "\n";
  stream << "* Envelope material: " << f_envelope_material->GetName() << "\n";
  stream << "* Surface  material: " << f_material->GetName() << "\n";
  if(f_probability_generated){
    stream << "* Size of probability facet vector " << f_probability.size() << "\n";
  }else{
    stream << "* Probability vector NOT generated\n";
  }
  stream << "**************************************************\n";
  stream << "**************************************************\n";
  stream << "\n";
  return stream.str();
}

G4String LogicalSurface::probability_information() const {
  std::stringstream stream;
  stream << "\n";
  stream << "**************************************************\n";
  stream << "*****  Information of LogicalSurface Object  *****\n";
  stream << "*****            - Probability -             *****\n";
  stream << "**************************************************\n";
  if(f_probability_generated){
    stream << "* Size of probability facet vector " << f_probability.size() << "\n";
    for(auto ele:f_probability){
      stream << "* " << ele << "\n";
    }
  }else{
    stream << "* Probability vector NOT generated\n";
  }
  stream << "**************************************************\n";
  stream << "**************************************************\n";
  stream << "\n";
  return stream.str();
}

G4String LogicalSurface::placed_elements_information() const {
  std::stringstream stream;
  stream << "\n";
  stream << "**************************************************\n";
  stream << "*****  Information of LogicalSurface Object  *****\n";
  stream << "*****          - placed elements -           *****\n";
  stream << "**************************************************\n";
  stream << "* Placed elements: nx=" << f_nx << " ny=" << f_ny << " total=" << f_nx*f_ny << "\n";
  for(G4int idx = 0; idx < f_nx * f_ny; idx++){
    auto *daughter = f_logical_envelope->GetDaughter(idx);
    stream << "* Name: " << daughter->GetName() << "\n";
    stream << "* Position: " << daughter->GetTranslation() << "\n";
  }
  stream << "**************************************************\n";
  stream << "**************************************************\n";
  stream << "\n";
  return stream.str();
}

void LogicalSurface::force_probability_generation() {
  f_logger.WriteInfo("Forcing generation of probability vector.");
  generate_probability();
}
void LogicalSurface::show_information() const {
  f_logger.WriteAlways(information());
}
void LogicalSurface::show_probability_information() const {
  f_logger.WriteAlways(probability_information());
}
void LogicalSurface::show_placed_elements_information() const {
  f_logger.WriteAlways(placed_elements_information());
}

}  // namespace LogicalSurface