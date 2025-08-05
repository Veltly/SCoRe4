/**
 * @brief definition of user ActionInitialization class
 * @file ActionInitialization.hh
 * @date 2025-08-05
 */

#ifndef ActionInitialization_MultiSurface_h
#define ActionInitialization_MultiSurface_h

#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
  ActionInitialization();
  ~ActionInitialization() override;

  void BuildForMaster() const override;
  void Build() const override;
};

#endif
