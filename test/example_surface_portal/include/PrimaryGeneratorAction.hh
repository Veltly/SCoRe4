/**
* @brief Primary generator action definition
* @file PrimaryGeneratorAction.hh
* @date 2025-10-27
*/

#ifndef PRIMARY_GENERATOR_ACTION_EXAMPLE_SURFACE_PORTAL_HH
#define PRIMARY_GENERATOR_ACTION_EXAMPLE_SURFACE_PORTAL_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "ParticleGenerator/include/MultiSubworldSampler.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"

class G4Event;
class G4Box;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event *) override;

private:
  Surface::MultiSubworldSampler f_sampler;
};

#endif //PRIMARY_GENERATOR_ACTION_EXAMPLE_SURFACE_PORTAL_HH
