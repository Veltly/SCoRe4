/**
 * @brief Definition of surface source class
 * @author C.Gruener
 * @date 2025-10-25
 * @file Source.hh
 */

#ifndef SURFACE_SOURCE_HH
#define SURFACE_SOURCE_HH

#include "G4GeneralParticleSource.hh"
#include "G4VPrimaryGenerator.hh"
#include "Shift.hh"
#include "Surface/SurfaceSourceStore.hh"

namespace Surface {
/**
 * @brief This class is a particle generator for simulating a surface contamination
 * @details Surfaces must be added to this source and are later sampled directly
 * from the logical volume.
 */
class Source : public G4VPrimaryGenerator {
 public:
  explicit Source(const G4String &name, Shift *shift, VerboseLevel verbose_lvl = VerboseLevel::Default);
  ~Source() override;
  void GeneratePrimaryVertex(G4Event *event) override;

 private:
  void generate_probability();
  size_t random_select_logical_surface_idx() const;

 private:
  G4GeneralParticleSource *f_particle_generator;
  G4String f_name;
  SurfaceSourceStore &f_store = SurfaceSourceStore::getInstance();
  Surface::Shift *f_shift{nullptr};
  std::vector<G4double> f_probability;
  G4bool f_probability_generated{false};
  Logger f_logger;
};
}  // namespace Surface

#endif  // SURFACE_SOURCE_HH
