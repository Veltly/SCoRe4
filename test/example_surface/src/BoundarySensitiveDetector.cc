/**
* @brief Implementation of Boundary Sensitive Detector class
* @file BoundarySensitiveDetector.cc
* @date 2025-11-02
 */

#include "BoundarySensitiveDetector.hh"
#include "Analysis.hh"

G4bool BoundarySensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *) {
    auto prePoint = step->GetPreStepPoint();
    auto postPoint = step->GetPostStepPoint();
    auto track = step->GetTrack();
    if (prePoint->GetPhysicalVolume()->GetName() == "Shell" ||
        postPoint->GetPhysicalVolume()->GetName() == "Shell") {
        const G4double kinE = prePoint->GetKineticEnergy();
        auto *analysis_manager = G4AnalysisManager::Instance();
        analysis_manager->FillH1(1, kinE);
        //analysis_manager->FillNtupleDColumn(0, kinE);
        //analysis_manager->AddNtupleRow();
        track->SetTrackStatus(fStopAndKill);
    }
    return true;
}
