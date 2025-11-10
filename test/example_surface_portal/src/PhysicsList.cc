/**
* @brief implementation of class physics list for example
* @file PhysicsList.hh
* @date 2025-10-27
 */
#include "PhysicsList.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "EmStandardPhysics_Green.hh"
#include "G4Alpha.hh"
#include "G4DecayPhysics.hh"
#include "G4Deuteron.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmLivermorePolarizedPhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmParameters.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysicsSS.hh"
#include "G4EmStandardPhysicsWVI.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4GenericIon.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsLEND.hh"
#include "G4HadronPhysicsShielding.hh"
#include "G4HadronPhysicsShieldingLEND.hh"
#include "G4HadronicInteraction.hh"
#include "G4HadronicInteractionRegistry.hh"
#include "G4HadronicParameters.hh"
#include "G4HadronicProcess.hh"
#include "G4HadronicProcessType.hh"
#include "G4He3.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonQMDPhysics.hh"
#include "G4Neutron.hh"
#include "G4NeutronHPThermalScattering.hh"
#include "G4NeutronHPThermalScatteringData.hh"
#include "G4OpticalPhysics.hh"
#include "G4ParticleHPManager.hh"
#include "G4PhysicsListHelper.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ProductionCutsTable.hh"
#include "G4StepLimiter.hh"
#include "G4StoppingPhysics.hh"
#include "G4String.hh"
#include "G4Triton.hh"
#include "G4Types.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include <assert.h>
#include <iostream>
#include <stddef.h>
#include <string>
//#include "../include/PhysicsList.hh"
//#include <G4DecayPhysics.hh>
//#include <G4RadioactiveDecayPhysics.hh>
//#include <G4EmStandardPhysics_option4.hh>
//#include "G4IonPhysics.hh"
//#include "../include/EmStandardPhysics_Green.hh"
//#include "G4SystemOfUnits.hh"
//
//PhysicsList::PhysicsList():G4VModularPhysicsList() {
//
//  SetVerboseLevel(1);
//
//  RegisterPhysics(new G4DecayPhysics(GetVerboseLevel()));
////  RegisterPhysics(new G4EmStandardPhysics_option4(GetVerboseLevel()));
//  RegisterPhysics(new EmStandardPhysics_Green(GetVerboseLevel()));
//  RegisterPhysics(new G4IonPhysics(GetVerboseLevel()));
//  RegisterPhysics(new G4RadioactiveDecayPhysics(GetVerboseLevel()));
//}
//
//void PhysicsList::SetCuts() {
//  G4VUserPhysicsList::SetCuts();
//  SetDefaultCutValue(0.1*mm);
//  DumpCutValuesTable();
//}


PhysicsList::PhysicsList(): G4VModularPhysicsList(){
  // EM Physics
  //  auto *emPhysics = new G4EmStandardPhysics_option4(GetVerboseLevel());
  auto *emPhysics = new EmStandardPhysics_Green(GetVerboseLevel());
  RegisterPhysics(emPhysics);
  G4EmParameters::Instance()->SetVerbose(GetVerboseLevel());
  // Activate LPM effect
  G4EmParameters::Instance()->SetLPM(true);
  G4EmParameters::Instance()->Dump();

  // Synchrotron Radiation & GN Physics
  G4EmExtraPhysics *emExtraPhysics = new G4EmExtraPhysics(GetVerboseLevel());
  RegisterPhysics(emExtraPhysics);

  // Decays
  RegisterPhysics(new G4DecayPhysics(GetVerboseLevel()));


  RegisterPhysics(new G4HadronElasticPhysicsHP(GetVerboseLevel()));


  G4VPhysicsConstructor *hpc = new G4HadronPhysicsShielding(
      "hInelastic Shielding", GetVerboseLevel(),
      G4HadronicParameters::Instance()->GetMinEnergyTransitionFTF_Cascade(),
      G4HadronicParameters::Instance()
          ->GetMaxEnergyTransitionFTF_Cascade());
  RegisterPhysics(hpc);


  G4ParticleHPManager::GetInstance()->SetProduceFissionFragments(true);

  // Stopping Physics
  RegisterPhysics(new G4StoppingPhysics(GetVerboseLevel()));

  // Ion Physics
  RegisterPhysics(new G4IonElasticPhysics(GetVerboseLevel()));
  RegisterPhysics(new G4IonQMDPhysics(GetVerboseLevel()));

  // Define convenient units for thermal neutron physics
  const G4double kel = 1. * CLHEP::kelvin;
  new G4UnitDefinition("millikelvin", "mK", "Temperature", kel / 1000.);
  new G4UnitDefinition("microkelvin", "uK", "Temperature", kel / 1000000.);
  new G4UnitDefinition("nanokelvin", "nK", "Temperature", kel / 1000000000.);

  // Decrease energy cut from default 990eV to 250eV
  // See https://geant4.web.cern.ch/node/1620
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(
      250 * CLHEP::eV, 100 * CLHEP::TeV);

  // Set default cut for all particles to 1nm.
  SetDefaultCutValue(1. * CLHEP::nm);

  // Set proton cut value to 0mm for producing low energy recoil nucleus
  SetCutValue(0. * CLHEP::mm, "proton");

  if (GetVerboseLevel() > 1) {
    G4VUserPhysicsList::DumpCutValuesTable();
  }
}

void PhysicsList::ConstructProcess() {
  // Call the method of the super class, because there the transport process is
  // added to the particles
  G4VModularPhysicsList::ConstructProcess();

  G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();
  G4StepLimiter *steplim =
      new G4StepLimiter(); // step limiter process activation
  ph->RegisterProcess(steplim, G4GenericIon::GenericIon());
  ph->RegisterProcess(steplim, G4He3::He3());
  ph->RegisterProcess(steplim, G4Alpha::Alpha());
  ph->RegisterProcess(steplim, G4Deuteron::Deuteron());
  ph->RegisterProcess(steplim, G4Triton::Triton());

  // ThermalNeutronScattering
  // added by Florian Stummer Dec 05, 2017
  // Get the elastic scattering process for neutrons
  G4ParticleDefinition *nPD = G4Neutron::Definition();
  G4ProcessVector *pvec = nPD->GetProcessManager()->GetProcessList();
  G4HadronicProcess *nElastic = 0;
  for (size_t i = 0; i < pvec->size(); i++) {
    if ((*pvec)[i]->GetProcessSubType() != fHadronElastic)
      continue;
    nElastic = dynamic_cast<G4HadronicProcess *>((*pvec)[i]);
    break;
  }
  // G4HadronicProcess* nElastic =
  // G4HadronicProcessStore::Instance()->FindProcess(nPD, fHadronElastic);
  assert(nElastic != 0);
  // Get the "regular" HP elastic scattering model, exclude thermal scattering
  // region
  G4HadronicInteraction *nElasticHP =
      G4HadronicInteractionRegistry::Instance()->FindModel("NeutronHPElastic");
  assert(nElasticHP != 0);
  nElasticHP->SetMinEnergy(4. * CLHEP::eV);

  // Attach HP thermal scattering model and data files to process
  nElastic->RegisterMe(new G4NeutronHPThermalScattering);
  nElastic->AddDataSet(new G4NeutronHPThermalScatteringData);
  // ThermalNeutronScattering - End
}