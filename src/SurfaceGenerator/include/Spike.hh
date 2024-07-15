// Copyright [2024] C.Gruener
// Date: 23-06-01

#ifndef SRC_SURFACEGENERATOR_INCLUDE_SPIKE_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_SPIKE_HH_

#include <vector>

#include "SurfaceGenerator/include/Storage.hh"

namespace Surface {

class Spike {
 public:
  enum class Spikeform { Pyramid, Bump, Peak };

 public:
  Spike(Spikeform, G4double aWidth_X, G4double aWidth_Y, G4double aHeight,
        G4int aNLayer);
  std::vector<Surface::SolidDescription> GetSpikeDescription();

 private:
  void GenerateSpike();
  void GeneratePyramide();
  void GenerateBump();
  void GeneratePeak();
  G4double FunctionBump(const G4double aNextHeight, const G4double aBaseside);
  G4double FunctionPeak(const G4double aNextHeight, const G4double aBaseside);

  Spikeform fSpikeform;
  G4double fWidth_X;
  G4double fWidth_Y;
  G4double fHeight;
  G4int fNLayer;
  const G4double fWidthTop_X{1e-6};
  const G4double fWidthTop_Y{1e-6};
  std::vector<Surface::SolidDescription> fSpikeDescription{};
};
}  // namespace Surface
#endif  // SRC_SURFACEGENERATOR_INCLUDE_SPIKE_HH_
