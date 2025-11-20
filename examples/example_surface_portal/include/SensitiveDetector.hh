/**
* @brief Definition of Sensitive Detector class
* @file SensitiveDetector.hh
* @date 2025-10-27
*/

#ifndef SURFACE_PORTAL_SENSITIVE_DETECTOR_HH
#define SURFACE_PORTAL_SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4StepPoint.hh"

class SensitiveDetector : public G4VSensitiveDetector {
 public:
  explicit SensitiveDetector(const G4String& name);

  void Initialize(G4HCofThisEvent*) override;

  G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

  void EndOfEvent(G4HCofThisEvent*) override;

 private:
  std::vector<G4double> f_edep;
};


#endif  // SURFACE_PORTAL_SENSITIVE_DETECTOR_HH
