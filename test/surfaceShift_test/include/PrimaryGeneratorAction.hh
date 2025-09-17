/**
 * @brief definition of the PrimaryGeneratorAction class
 * @file PrimaryGeneratorAction.hh
 * @date 2025-08-05
 */

#ifndef PrimaryGeneratorAction_surfaceShift_hh
#define PrimaryGeneratorAction_surfaceShift_hh

#include <G4GeneralParticleSource.hh>

#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "ParticleGenerator/include/Shift.hh"
#include "globals.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction() override;

  void GeneratePrimaries(G4Event *) override;

private:
  G4GeneralParticleSource *fGps;
  Surface::Shift *fShiftTable; // definition of shift table for vertex-shift
};

#endif
