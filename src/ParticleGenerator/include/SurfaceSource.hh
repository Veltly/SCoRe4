//
//
//
//
//      Author: C.Gruener
//
//

#ifndef SURFACE_SURFACESOURCE
#define SURFACE_SURFACESOURCE

#include "../../SurfaceGenerator/include/FacetStore.hh"
#include "G4VPrimaryGenerator.hh"
#include "SurfaceSourceMessenger.hh"

class G4Event;
class G4Navigator;
class G4VPhysicalVolume;
class G4PhysicalVolumeStore;
class G4GeneralParticleSource;

namespace Surface {

//@brief Generates a particle as a primary event on the surface.
// Surface has to be detected before first event can be executed.

class SurfaceSource : public G4VPrimaryGenerator {

public:
  SurfaceSource();
  ~SurfaceSource();
  void GeneratePrimaryVertex(G4Event *argEvent);
  void ShowSurface();
  void LogSurface(G4String &aFilename);

private:
  Surface::SurfaceSourceMessenger *fMessenger;
  G4GeneralParticleSource *fParticleGenerator;
  Surface::FacetStore fFacetStore;
  G4bool shift;
};
} // namespace Surface
#endif // SURFACE_SURFACESOURCE
