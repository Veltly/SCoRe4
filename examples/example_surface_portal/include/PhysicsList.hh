/**
* @brief physics list for example
* @file PhysicsList.hh
* @date 2025-10-27
*/

#ifndef PHYSICS_LIST_EXAMPLE_SURFACE_PORTAL_HH
#define PHYSICS_LIST_EXAMPLE_SURFACE_PORTAL_HH

#include <G4VModularPhysicsList.hh>

class PhysicsList : public G4VModularPhysicsList {

public:
  PhysicsList();
  ~PhysicsList() override = default;
  void ConstructProcess();

};

#endif // PHYSICS_LIST_EXAMPLE_SURFACE_PORTAL_HH
