/**
 * @brief definition of ActionInitialization class for user actions
 * @file ActionInitialization.hh
 * @date 2025-10-27
 */

#ifndef ACTION_INITIALIZATION_EXAMPLE_SURFACE_HH
#define ACTION_INITIALIZATION_EXAMPLE_SURFACE_HH

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
 public:
  ActionInitialization();
  ~ActionInitialization() override;

  void BuildForMaster() const override;
  void Build() const override;
};

#endif
