/**
 * @brief Definition of class MultiSubwordlSampler
 * @author C.Gruener
 * @date 2024-06-18
 * @file MultiSubworldSampler.hh
 */

#ifndef SRC_PARTICLEGENERATOR_INCLUDE_MULTISUBWORLDSAMPLER_HH
#define SRC_PARTICLEGENERATOR_INCLUDE_MULTISUBWORLDSAMPLER_HH

#include "G4GeneralParticleSource.hh"
#include "G4VPrimaryGenerator.hh"
#include "ParticleGenerator/include/PointShift.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Service/include/VSampler.hh"

namespace Surface {

struct Coord {
  G4int x;
  G4int y;
};

std::ostream &operator<<(std::ostream &os, const Coord &coord);

class MultiSubworldSampler : public G4VPrimaryGenerator {
 public:
  MultiSubworldSampler(G4String name, G4String portalName,
                       const G4String &shiftFilename,
                       VerboseLevel verboseLvl = VerboseLevel::Default);

  MultiSubworldSampler(G4String name, G4String portalName,
                       VerboseLevel verboseLvl = VerboseLevel::Default);
  ~MultiSubworldSampler() override;
  void GeneratePrimaryVertex(G4Event *argEvent) override;

  void SetSubworld(SubworldGrid<MultipleSubworld> *);

  inline G4bool IsSamplerReady() const { return fSamplerReady; }

 private:
  void PrepareSampler();
  G4ThreeVector GetRandom();
  std::string Information() const;

 private:
  const G4String fName;
  const G4String fPortalName;
  SubworldGrid<MultipleSubworld> *fSubworld{nullptr};
  Surface::PointShift fShift;
  const G4bool fShiftActive;
  VSampler<Coord> fSubworldSampler;
  G4bool fSamplerReady;
  Logger fLogger;
  G4GeneralParticleSource *fParticleGenerator;
};
}  // namespace Surface

#endif  // SRC_PARTICLEGENERATOR_INCLUDE_MULTISUBWORLDSAMPLER_HH
