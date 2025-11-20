//
// Created by chris on 01.11.25.
//
#include "RunAction.hh"
#include "Analysis.hh"
#include "G4SystemOfUnits.hh"

RunAction::~RunAction(){
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run *) {
  auto analysis_manager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysis_manager->GetType() << " analysis manager." << G4endl;

  analysis_manager->OpenFile("DepositedEnergyOutput");

  analysis_manager->SetVerboseLevel(1);

  const G4double max_energy = 6. * MeV;
  const G4double min_energy = 0 * eV;
  const G4int bins = 300;
  analysis_manager->CreateH1("Cube", "Deposited Energy in Cube", bins, min_energy, max_energy);
  analysis_manager->CreateH1("Shell", "Deposited Energy in Shell", bins,min_energy,max_energy);
}

void RunAction::EndOfRunAction(const G4Run *) {
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->Write();
  analysis_manager->CloseFile();
}