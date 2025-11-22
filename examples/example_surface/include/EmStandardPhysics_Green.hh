// author: C.Gruener
// date: 12.03.2024
//
// EmStandardPhysics_Green is a variation of G4EmStandardPhysics_option4

#ifndef EmStandardPhysics_Green_HH
#define EmStandardPhysics_Green_HH

#include "G4EmParticleList.hh"
#include "G4VPhysicsConstructor.hh"


class EmStandardPhysics_Green : public G4VPhysicsConstructor {
public:
  explicit EmStandardPhysics_Green(G4int ver = 1, const G4String &name = "");

  ~EmStandardPhysics_Green() override;

  void ConstructParticle() override;
  void ConstructProcess() override;

private:
  G4int verbose;
  G4EmParticleList partList;
};


#endif
