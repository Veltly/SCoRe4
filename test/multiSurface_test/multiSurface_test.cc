/**
 * @brief Testsetup for MultiSurface simulation
 * @file multiSurface_test.cc
 * @author C.Gruener
 * @date 2024-06-11
 */

#include "include/DetectorConstruction.hh"
#include "include/PhysicsList.hh"
#include "ActionInitialization.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"


int main(int argc, char **argv) {
  G4cout << "MultiSurface test application starting ..." << G4endl;

  G4UIExecutive *ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv, "tcsh");
  }

  // Construct the default run manager
  auto *runManager = new G4RunManager;

  // Initialize visualization
  G4VisManager *visManager = new G4VisExecutive();
  visManager->Initialize();

  // Physics list
  runManager->SetUserInitialization(new PhysicsList());
  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  // Get the pointer to the User Interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if (!ui) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  } else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute macros/init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
}
