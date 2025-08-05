/**
 * @brief definition of ActionInitialization class for user actions
 * @file ActionInitialization.hh
 * @date 2025-08-05
 */

#ifndef ActionInitialization_surfaceShift_hh
#define ActionInitialization_surfaceShift_hh

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
 public:
  ActionInitialization();
  ~ActionInitialization() override;

  void BuildForMaster() const override;
  void Build() const override;
};

#endif
