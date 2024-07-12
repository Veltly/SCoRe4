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
  G4ThreeVector GetRandom();

public:
  MultiSubworldSampler(const G4String &name, const G4String &portalName,
                       const G4String &shiftFilename,
                       const G4int verboseLvl = 6);
  MultiSubworldSampler(const G4String &name, const G4String &portalName,
                       const G4int verboseLvl = 6);

  void GeneratePrimaryVertex(G4Event *argEvent);
  inline G4bool IsSamplerReady() const { return fSamplerReady; };
  void SetSubworld(SubworldGrid<MultipleSubworld> *);

private:
  G4String fName;
  G4String fPortalName;
  SubworldGrid<MultipleSubworld> *fSubworld;
  Surface::Shift fShift;
  G4bool fShiftActive;
  VSampler<Coord> fSubworldSampler;
  G4bool fSamplerReady;
  Logger fLogger;
  G4GeneralParticleSource *fParticleGenerator;
};
} // namespace Surface

#endif // MULTISUBWORLDSAMPLER
