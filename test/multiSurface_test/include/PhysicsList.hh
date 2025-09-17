/**
* @brief define PhysicsList class
* @file PhysicsList.hh
* @date 2025-08-05
*/

#ifndef Physics_list_hh
#define Physics_list_hh

#include <G4VModularPhysicsList.hh>

class PhysicsList : public G4VModularPhysicsList {

public:
  PhysicsList();
  ~PhysicsList() override = default;
  void ConstructProcess() override;
  void SetCuts() override;
};

#endif
