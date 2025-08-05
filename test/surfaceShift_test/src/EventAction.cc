/**
 * @brief implementation of eventAction class to fill a 1D histogram in shift direction
 * @file EventAction.cc
 * @date 2025-08-05
 */

#include "EventAction.hh"
#include "G4CsvAnalysisManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"

EventAction::EventAction() : G4UserEventAction() {}

EventAction::~EventAction() = default;

void EventAction::BeginOfEventAction(const G4Event *) {}

void EventAction::EndOfEventAction(const G4Event *event) {
  G4CsvAnalysisManager *analysis = G4CsvAnalysisManager::Instance();
  G4ThreeVector primaryPosition = event->GetPrimaryVertex()->GetPosition();
  analysis->FillH1(1, abs(primaryPosition.z()));
}
