#include "PrimaryGeneratorAction.hh"
#include "../../../src/ParticleGenerator/include/MultiSubworldSampler.hh"
#include "../../../src/ParticleGenerator/include/SurfaceSource.hh"
#include "G4RunManager.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/PortalStore.hh"
#include "Service/include/Locator.hh"
#include <G4GeneralParticleSource.hh>
PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(),
      fSampler(Surface::MultiSubworldSampler{"macros/shiftTable"}) {}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  fSampler.GeneratePrimaryVertex(anEvent);

  //  if (!fSampler.GetSamplerStatus()) {
  //    Surface::PortalStore pStore = Surface::Locator::GetPortalStore();
  //    Surface::MultipleSubworld *subworld =
  //        static_cast<Surface::MultipleSubworld *>(pStore[0]);
  //    fSampler.SetSubworld(subworld->GetSubworldGrid());
  //  }
  //  fGps->GeneratePrimaryVertex(anEvent);
  //  G4ThreeVector position = fSampler.GetRandom();
  //  anEvent->GetPrimaryVertex()->SetPosition(position.x(), position.y(),
  //                                           position.z());
}
