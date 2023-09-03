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
#include "../include/SurfaceSourceMessenger.hh"
#include "G4VPrimaryGenerator.hh"

class G4Event;
class G4Navigator;
class G4VPhysicalVolume;
class G4PhysicalVolumeStore;
class G4GeneralParticleSource;

namespace Surface {

class SurfaceSourceMessenger;
class FacetStore;

//@brief Generates a particle as a primary event on the surface.
// Surface has to be detected before first event can be executed.

class SurfaceSource : public G4VPrimaryGenerator {

public:
  SurfaceSource();
  ~SurfaceSource();
  void GeneratePrimaryVertex(G4Event *argEvent);
  void SetFacetStore(Surface::FacetStore &);

private:
  G4GeneralParticleSource *fParticleGenerator;
  Surface::SurfaceSourceMessenger *Messenger{nullptr};
  Surface::FacetStore fFacetStore;
};
} // namespace Surface
#endif /* SURFACE_SURFACESOURCE_HH_ */
