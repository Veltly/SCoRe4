/**
 * @brief define physics for test-simulation
 * @file PhysicsList.cc
 * @date 2025-08-05
 */

#include "../include/PhysicsList.hh"
#include <G4Alpha.hh>
#include <G4DecayPhysics.hh>
#include <G4Deuteron.hh>
#include <G4EmStandardPhysics_option4.hh>
#include <G4Gamma.hh>
#include <G4GenericIon.hh>
#include <G4He3.hh>
#include <G4PhysicsListHelper.hh>
#include <G4StepLimiter.hh>
#include <G4VModularPhysicsList.hh>

PhysicsList::PhysicsList() : G4VModularPhysicsList() {
  RegisterPhysics(new G4EmStandardPhysics_option4());
  RegisterPhysics(new G4DecayPhysics());
}

void PhysicsList::SetCuts() {
  G4VUserPhysicsList::SetCuts();
  DumpCutValuesTable();
}

void PhysicsList::ConstructProcess() {
  G4VModularPhysicsList::ConstructProcess();
  G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();
  auto *stepLimit = new G4StepLimiter();
  ph->RegisterProcess(stepLimit, G4GenericIon::GenericIon());
  ph->RegisterProcess(stepLimit, G4He3::He3());
  ph->RegisterProcess(stepLimit, G4Alpha::Alpha());
  ph->RegisterProcess(stepLimit, G4Deuteron::Deuteron());
  ph->RegisterProcess(stepLimit, G4Gamma::Gamma());
}
