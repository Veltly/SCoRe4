/**
* @brief Implementation of Sensitive Detector class
* @file SensitiveDetector.cc
* @date 2025-10-27
 */

#include "SensitiveDetector.hh"
#include "G4UnitsTable.hh"
#include "Analysis.hh"
#include <numeric>

SensitiveDetector::SensitiveDetector(const G4String& name) : G4VSensitiveDetector(name) {
  f_edep.assign(3, 0.0); // store for 3 volumes
}

void SensitiveDetector::Initialize(G4HCofThisEvent*){
  std::fill(f_edep.begin(), f_edep.end(), 0.0);
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*){
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep == 0.) return false;

  G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
  if (copyNo >= 0 && copyNo < (G4int)f_edep.size())
    f_edep[copyNo] += edep;
  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*){
  auto *analysis_manager = G4AnalysisManager::Instance();
  const auto total_edep = std::accumulate(f_edep.begin(),f_edep.end(),0.0);
  analysis_manager->FillH1(0, total_edep);
}