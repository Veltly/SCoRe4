#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "../../../src/ParticleGenerator/include/SurfaceSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4Event;
class G4Box;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event *) override;

private:
  Surface::SurfaceSource *fSurfaceSource;
};

#endif
