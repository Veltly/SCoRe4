/**
* @brief Implementation of Boundary Sensitive Detector class
* @file BoundarySensitiveDetector.cc
* @date 2025-11-02
 */

#include "BoundarySensitiveDetector.hh"
#include "Analysis.hh"
#include "G4SystemOfUnits.hh"

void BoundarySensitiveDetector::Initialize(G4HCofThisEvent *) {
  f_total_edep = 0;
}

G4bool BoundarySensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *) {
    auto prePoint = step->GetPreStepPoint();
    auto postPoint = step->GetPostStepPoint();
    auto track = step->GetTrack();
    if (prePoint->GetPhysicalVolume()->GetName() == "Shell" ||
        postPoint->GetPhysicalVolume()->GetName() == "Shell") {
        const G4double kinE = prePoint->GetKineticEnergy();
        f_total_edep += kinE;
        track->SetTrackStatus(fStopAndKill);
    }
    return true;
}

void BoundarySensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
  auto *analysis_manager = G4AnalysisManager::Instance();
  if(f_total_edep != 0) {
    analysis_manager->FillH1(1, f_total_edep);
  }
}