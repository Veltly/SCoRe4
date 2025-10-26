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

Source::Source(const G4String &name, VerboseLevel verbose_lvl)
    :fParticleGenerator(new G4GeneralParticleSource),
      f_name(name),
      f_logger(name, verbose_lvl) {
  f_logger.WriteDetailInfo("Instantiated Source " + f_name);
}

void Source::generate_probability() {
  f_logger.WriteDebugInfo("Setup of probability vector");
  const auto size = f_store.size();
  std::vector<G4double> areas;
  areas.reserve(size);
  for (size_t idx = 0; idx < size; idx++) {
    auto *volume = dynamic_cast<LogicalSurface *>(f_store.get_volume(idx));
    areas.push_back(volume->surface_area());
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
  for (int i = 0; i < event->GetNumberOfPrimaryVertex(); i++) {
    const size_t idx = random_select_logical_surface_idx();
    auto *volume = dynamic_cast<LogicalSurface *>(f_store.get_volume(idx));
    G4ThreeVector point{};
    G4ThreeVector direction{};
    volume->sample_point(point, direction);
    point += f_store.get_position(idx);
    //auto *rotation_matrix = f_store.get_rotation(idx); // needed for shift
    //direction = (*rotation_matrix) * direction;
    fParticleGenerator->GeneratePrimaryVertex(event);
    event->GetPrimaryVertex(i)->SetPosition(point.x(), point.y(), point.z());
    f_logger.WriteDebugInfo("Set point for primary vertex: ", point);
  }
}
} // namespace Surface
