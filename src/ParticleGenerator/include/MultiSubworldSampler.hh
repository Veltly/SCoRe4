// Author: C.Gruener
// Date: 24-06-18
// File: MultiSubworldSampler

#ifndef MULTISUBWORLDSAMPLER_HH
#define MULTISUBWORLDSAMPLER_HH

#include "../../Portal/include/MultipleSubworld.hh"
#include "../../Portal/include/SubworldGrid.hh"
#include "../../Service/include/VSampler.hh"
#include "G4GeneralParticleSource.hh"
#include "G4VPrimaryGenerator.hh"
#include "ShiftTable.hh"
namespace Surface {

struct Coord {
  G4int x;
  G4int y;
};

class MultiSubworldSampler : public G4VPrimaryGenerator {
private:
  void PrepareSampler();

public:
  MultiSubworldSampler(G4String);
  void GeneratePrimaryVertex(G4Event *argEvent);
  G4ThreeVector GetRandom();
  inline G4bool GetSamplerStatus() const { return fSamplerReady; };
  void SetSubworld(SubworldGrid<MultipleSubworld> *);

private:
  SubworldGrid<MultipleSubworld> *fSubworld;
  Surface::Shift fShift;
  VSampler<Coord> fSubworldSampler;
  G4bool fSamplerReady;
  Logger fLogger;
  G4GeneralParticleSource *fParticleGenerator;
};
} // namespace Surface

#endif // MULTISUBWORLDSAMPLER
