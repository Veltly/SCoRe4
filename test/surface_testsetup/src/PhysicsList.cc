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
