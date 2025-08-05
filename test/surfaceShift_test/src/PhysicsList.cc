/**
* @brief definition of user defined physics list
* @file PhysicsList.cc
* @date 2025-08-05
*/

#include "../include/PhysicsList.hh"
#include <G4DecayPhysics.hh>
#include <G4EmStandardPhysics_option4.hh>

PhysicsList::PhysicsList() {
  RegisterPhysics(new G4EmStandardPhysics_option4());
  RegisterPhysics(new G4DecayPhysics());
}

void PhysicsList::SetCuts() {
  G4VUserPhysicsList::SetCuts();
  DumpCutValuesTable();
}
