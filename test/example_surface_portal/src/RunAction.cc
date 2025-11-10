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
  //analysis_manager->SetNtupleMerging(true);

  analysis_manager->CreateH1("Cube", "Deposited Energy in Cube", 100, 0., 10*MeV);
  analysis_manager->CreateH1("Shell", "Deposited Energy in Shell", 100,0.,10.*MeV);

  //analysis_manager->CreateNtuple("Energy", "Energy per event");
  //analysis_manager->CreateNtupleDColumn("Cube");
  //analysis_manager->FinishNtuple();


}

void RunAction::EndOfRunAction(const G4Run *) {
  auto analysis_manager = G4AnalysisManager::Instance();
  analysis_manager->Write();
  analysis_manager->CloseFile();
}