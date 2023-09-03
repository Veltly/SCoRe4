//
//
//
//
//	author: C.Gruener
//
//

#ifndef SURFACE_SPIKE
#define SURFACE_SPIKE

#include "Storage.hh"
#include <vector>

namespace Surface {

class Spike {
public:
  enum class Spikeform{Pyramid, Bump, Peak};
public:
  Spike(Spikeform, G4double aWidth_X, G4double aWidth_Y, G4double aHeight, G4int aNLayer);
  std::vector<Surface::SolidDescription> GetSpikeDescription();

private:
  void GenerateSpike();
  void GeneratePyramide();
  void GenerateBump();
  void GeneratePeak();
  
  Spikeform fSpikeform;
  G4double fWidth_X;
  G4double fWidth_Y;
  G4double fHeight;
  G4int fNLayer;
  const G4double fWidthTop_X{1e-12};
  const G4double fWidthTop_Y{1e-12};
  std::vector<Surface::SolidDescription> fSpikeDescription{};

};
} // namespace Surface
#endif
