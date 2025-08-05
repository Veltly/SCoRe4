/**
 * @brief definition of the PrimaryGeneratorAction class for sampling of vertices
 * @file PrimaryGeneratorAction.hh
 * @date 2025-08-04
 * @author C.Gruener
 */

#ifndef PrimaryGeneratorAction_MultiSurface_hh
#define PrimaryGeneratorAction_MultiSurface_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "ParticleGenerator/include/MultiSubworldSampler.hh"
#include <G4GeneralParticleSource.hh>

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event *) override;

private:
  Surface::MultiSubworldSampler fSampler; // samples vertices on and below multisurface
};

#endif
