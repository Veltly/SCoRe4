/**
* @brief Definition of Boundary Sensitive Detector class
* @file BoundarySensitiveDetector.hh
* @date 2025-11-02
 */

#ifndef EXAMPLE_SURFACE_BOUNDARYSENSITIVEDETECTOR_HH
#define EXAMPLE_SURFACE_BOUNDARYSENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4StepPoint.hh"

class BoundarySensitiveDetector : public G4VSensitiveDetector {
 public:
  explicit BoundarySensitiveDetector(const G4String& name) : G4VSensitiveDetector(name){};

  void Initialize(G4HCofThisEvent*) override;
  G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;
  void EndOfEvent(G4HCofThisEvent*) override;

 private:
  G4double f_total_edep;
};





#endif  // EXAMPLE_SURFACE_BOUNDARYSENSITIVEDETECTOR_HH
