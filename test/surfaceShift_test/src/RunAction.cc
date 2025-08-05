/**
* @brief definition of RunAction
* @file RunAction.cc
* @date 2025-08-05
*/

#include "RunAction.hh"
#include <G4CsvAnalysisManager.hh>
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "PrimaryGeneratorAction.hh"

/**
 * @brief setup of 1D histogram from 0 t0 200 nm with 2000 bins to analys shift
 */
RunAction::RunAction() : G4UserRunAction() {
  G4CsvAnalysisManager *analysis = G4CsvAnalysisManager::Instance();
  analysis->SetVerboseLevel(1);
  analysis->SetFirstHistoId(1);
  analysis->CreateH1("depth", "position", 2000, 0,
                     0.0002);  // histogram from 0 to 200 nm
  analysis->OpenFile("depth");
  G4cout << "analysis of shift class" << G4endl;
}

RunAction::~RunAction() {
  G4CsvAnalysisManager *analysis = G4CsvAnalysisManager::Instance();
  analysis->Write();
  analysis->CloseFile();
}
