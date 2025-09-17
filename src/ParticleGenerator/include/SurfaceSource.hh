/**
 * @brief Defines class for generating a particle source for a rough surface
 * @author C.Gruener
 * @date 2023-06-01
 * @file SurfaceSource.hh
 */

#ifndef SRC_PARTICLEGENERATOR_INCLUDE_SURFACESOURCE_HH
#define SRC_PARTICLEGENERATOR_INCLUDE_SURFACESOURCE_HH

#include "G4VPrimaryGenerator.hh"
#include "ParticleGenerator/include/SurfaceSourceMessenger.hh"
#include "SurfaceGenerator/include/FacetStore.hh"

class G4Event;
class G4Navigator;
class G4VPhysicalVolume;
class G4GeneralParticleSource;

namespace Surface {
/**
 * @brief Generates a particle as a primary event on the surface.
 * @details Surface has to be detected before first event can be executed.
 */

class SurfaceSource : public G4VPrimaryGenerator {
 public:
  SurfaceSource();
  ~SurfaceSource() override;
  void GeneratePrimaryVertex(G4Event *argEvent) override;
  void ShowSurface();
  void LogSurface(const G4String &aFilename);

 private:
  Surface::SurfaceSourceMessenger *fMessenger;
  G4GeneralParticleSource *fParticleGenerator;
  Surface::FacetStore fFacetStore;
};
}  // namespace Surface
#endif  // SRC_PARTICLEGENERATOR_INCLUDE_SURFACESOURCE_HH
