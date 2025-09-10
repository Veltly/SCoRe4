/**
 * @brief
 * @file Spike.hh
 * @date 2023-06-01
 * @author C.Gruener
 */

#ifndef SRC_SURFACE_GENERATOR_INCLUDE_SPIKE_HH_
#define SRC_SURFACE_GENERATOR_INCLUDE_SPIKE_HH_

#include <vector>
#include "SurfaceGenerator/include/Storage.hh"

namespace Surface {

/**
 * @brief Generates the description of a whole spike.
 * @details A spike can have multiple layers which are stored in the fSpikeDescription vector
 * Contains information like, which G4Solid to use, the parameters of this solid, the
 * location in the spike, and the ids of the outer surface.
 * The description can be understood by the Assembler class.
 */
class Spike {
 public:
  enum class Spikeform { Pyramid, Bump, Peak };

 public:
  Spike(Spikeform, G4double aWidth_X, G4double aWidth_Y, G4double aHeight,
        G4int aNLayer);
  std::vector<Surface::SolidDescription> GetSpikeDescription();

 private:
  void GenerateSpike();
  void GeneratePyramid();
  void GenerateBump();
  void GeneratePeak();
  G4double FunctionBump(G4double aNextHeight, G4double aBaseSide) const;
  G4double FunctionPeak(G4double aNextHeight, G4double aBaseSide) const;

  Spikeform fSpikeform;
  G4double fWidth_X;
  G4double fWidth_Y;
  G4double fHeight;
  G4int fNLayer;
  const G4double fWidthTop_X{1e-3 * CLHEP::nm};
  const G4double fWidthTop_Y{1e-3 * CLHEP::nm};
  std::vector<Surface::SolidDescription> fSpikeDescription{};
};
}
#endif
