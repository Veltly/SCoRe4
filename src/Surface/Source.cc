/**
* @brief Implementation of surface Source
* @author C.Gruener
* @date 2025-10-25
* @file Source.cc
*/

#include "Source.hh"
#include "LogicalSurface.hh"
#include "Randomize.hh"
#include <vector>
#include <numeric>

namespace Surface {

Source::Source(const G4String &name, Shift * shift, VerboseLevel verbose_lvl)
    :f_particle_generator(new G4GeneralParticleSource()),
      f_name(name),
      f_shift(shift),
      f_logger(name, verbose_lvl) {
  f_logger.WriteDetailInfo("Instantiated Source " + f_name);
}

void Source::generate_probability() {
  f_logger.WriteDebugInfo("Setup of probability vector");
  const auto size = f_store.size();
  std::vector<G4double> areas;
  areas.reserve(size);
  for (size_t idx = 0; idx < size; idx++) {
    auto *volume = f_store.get_volume(idx);
    areas.push_back(volume->surface_area());
  }
  f_probability.clear();
  f_probability.reserve(size);
  const G4double total_area = std::accumulate(areas.begin(), areas.end(), 0.);
  G4double cumulative_sum{0};
  for (auto area : areas) {
    cumulative_sum += area;
    if(total_area <= 0){
      G4String error_msg = "Total area of surface <= 0!";
      G4Exception("Source::generate_probability()",
                  "", FatalException,
                  error_msg);
    }
    f_probability.push_back(cumulative_sum / total_area);
  }
  f_probability_generated = true;
}

size_t Source::random_select_logical_surface_idx() const {
  const G4double random = G4UniformRand();
  const size_t size = f_probability.size();
  for (size_t idx = 0; idx < size; idx++) {
    if (random <= f_probability[idx]) {
      f_logger.WriteDebugInfo("Sampling idx from probability vector.");
      return idx;
    }
  }
  exit(EXIT_FAILURE);  // should never happen
}

void Source::GeneratePrimaryVertex(G4Event *event) {
  if (not f_probability_generated) {
    generate_probability();
  }

  const size_t idx = random_select_logical_surface_idx();
  auto *volume = dynamic_cast<LogicalSurface *>(f_store.get_volume(idx));
  G4ThreeVector point{};
  G4ThreeVector direction{};
  volume->sample_point(point, direction);
  auto *rotation_matrix = f_store.get_rotation(idx);
  point.transform(rotation_matrix->inverse()); //inverse because I go from local to global
  direction.transform(rotation_matrix->inverse());
  point += f_store.get_position(idx);
  f_particle_generator->GeneratePrimaryVertex(event);
  if(f_shift != nullptr){
    f_shift->DoShift(point, direction);
  }
  event->GetPrimaryVertex(0)->SetPosition(point.x(), point.y(), point.z());
  f_logger.WriteDebugInfo("Set point for primary vertex: ", point);
}

Source::~Source() {
  delete f_particle_generator;
  if(f_shift != nullptr){
    delete f_shift;
    f_shift = nullptr;
  }
}
} // namespace Surface
