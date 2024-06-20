// Author: C.Gruener
// Date: 24-06-18
// File: MultiSubworldSampler

#ifndef MULTISUBWORLDSAMPLER_HH
#define MULTISUBWORLDSAMPLER_HH

#include "../../Service/include/VSampler.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/SubworldGrid.hh"
#include "ShiftTable.hh"

namespace Surface {

struct Coord {
  G4int x;
  G4int y;
};

class MultiSubworldSampler {
private:
  void PrepareSampler();

public:
  MultiSubworldSampler(G4String);

  G4ThreeVector GetRandom();
  inline G4bool GetSamplerStatus() const { return fSamplerReady; };
  void SetSubworld(SubworldGrid<MultipleSubworld> *);

private:
  SubworldGrid<MultipleSubworld> *fSubworld;
  Surface::Shift fShift;
  VSampler<Coord> fSubworldSampler;
  G4bool fSamplerReady;
  Logger fLogger;
};
} // namespace Surface

#endif // MULTISUBWORLDSAMPLER
