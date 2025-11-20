/**
 * @brief Example implementation of surface using a portal
 * @author C.Gruener
 * @date 2025-10-25
 * @file example_surface_portal.cc
 */

#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "RunAction.hh"

int main(int argc, char **argv) {
  G4cout << "Surface test application starting ..." << G4endl;
  G4UIExecutive *ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  if (argc == 2){
    const G4String command = argv[1];
    ui = new G4UIExecutive(argc, argv, command);
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
  runManager->SetUserInitialization(new ActionInitialization());
  runManager->SetUserAction(new RunAction());
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
