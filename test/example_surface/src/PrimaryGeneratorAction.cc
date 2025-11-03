/**
* @brief Implementation of Primary generator action class
* @file PrimaryGeneratorAction.hh
* @date 2025-10-27
 */

#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "Surface/Shift.hh"
#include "G4SystemOfUnits.hh"

//PrimaryGeneratorAction::PrimaryGeneratorAction()
//    : G4VUserPrimaryGeneratorAction(),
//      f_surface_source(new Surface::Source("SurfaceSource",
//                                           nullptr,
//                                           Surface::VerboseLevel::Default)
//                       ){}

//PrimaryGeneratorAction::~PrimaryGeneratorAction()= default;
//
//void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
//  f_surface_source->GeneratePrimaryVertex(anEvent);
//}

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  // Create a particle gun that will shoot ONE particle per event
  fParticleGun = new G4ParticleGun(1);

  // Find the alpha particle definition
  G4ParticleDefinition* particle
      = G4ParticleTable::GetParticleTable()->FindParticle("alpha");

  // Configure the gun
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(5.0 * MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.2, 0.2, -10.*cm)); // start 10 cm before the cube
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.)); // shoot along +z
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Tell Geant4 to create the primary particle for this event
  fParticleGun->GeneratePrimaryVertex(anEvent);
}