// Author: C.Gruener
// Date: 24-07-09
// File: Shift Messenger
//

#include "../include/ShiftTableMessenger.hh"
#include "../include/ShiftTable.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

#include <G4ApplicationState.hh>

Surface::ShiftMessenger::ShiftMessenger(Surface::Shift *shift)
    : fShift(shift), fDirectory(nullptr), fCmdVerbose(nullptr),
      fCmdPrintShiftTable(nullptr), fCmdLoadShiftTable(nullptr),
      fCmdSetMinShift(nullptr), fCmdSetMaxShift(nullptr),
      fCmdConfineToMaterial(nullptr) {
  // As the messenger class is instantiated by the PrimaryGeneratorSource, the
  // pointer Source can not be NULL, hence no test needed.

  fDirectory = new G4UIdirectory("/Shift/");
  fDirectory->SetGuidance("Controls the shift of the particle source");
  /** @todo Specify the state for which the commands are available. **/

  fCmdVerbose = new G4UIcmdWithAnInteger("/Shift/Verbose", this);
  fCmdVerbose->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fCmdVerbose->SetGuidance("Set verbose level of shift");

  fCmdPrintShiftTable =
      new G4UIcmdWithoutParameter("/Shift/PrintShiftTable", this);
  fCmdPrintShiftTable->AvailableForStates(G4State_PreInit, G4State_Init,
                                          G4State_Idle);
  fCmdPrintShiftTable->SetGuidance("Print ShiftTable to output");

  fCmdLoadShiftTable = new G4UIcmdWithAString("/Shift/LoadShiftTable", this);
  fCmdLoadShiftTable->AvailableForStates(G4State_PreInit, G4State_Init,
                                         G4State_Idle);
  fCmdLoadShiftTable->SetGuidance("Load a ShiftTable from filepath");

  fCmdSetMinShift = new G4UIcmdWithADoubleAndUnit("/Shift/SetMinShift", this);
  fCmdSetMinShift->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetMinShift->SetGuidance("Set the minimal shift to do");
  fCmdSetMinShift->SetDefaultUnit("mm");
  fCmdSetMinShift->SetDefaultValue(0.);

  fCmdSetMaxShift = new G4UIcmdWithADoubleAndUnit("/Shift/SetMaxShift", this);
  fCmdSetMaxShift->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetMaxShift->SetGuidance("Set the maximal shift to do");
  fCmdSetMaxShift->SetDefaultUnit("mm");
  fCmdSetMaxShift->SetDefaultValue(DBL_MAX);

  fCmdConfineToMaterial =
      new G4UIcmdWithAString("/Shift/ConfineToMaterial", this);
  fCmdConfineToMaterial->AvailableForStates(G4State_PreInit, G4State_Init,
                                            G4State_Idle);
  fCmdConfineToMaterial->SetGuidance("Confine shift point to material");
  fCmdConfineToMaterial->SetDefaultValue("");
}

Surface::ShiftMessenger::~ShiftMessenger() {
  delete fDirectory;
  fDirectory = nullptr;
  delete fCmdVerbose;
  fCmdVerbose = nullptr;
  delete fCmdPrintShiftTable;
  fCmdPrintShiftTable = nullptr;
  delete fCmdLoadShiftTable;
  fCmdLoadShiftTable = nullptr;
  delete fCmdSetMinShift;
  fCmdSetMinShift = nullptr;
  delete fCmdSetMaxShift;
  fCmdSetMaxShift = nullptr;
  delete fCmdConfineToMaterial;
  fCmdConfineToMaterial = nullptr;
}

void Surface::ShiftMessenger::SetNewValue(G4UIcommand *command,
                                          G4String newValues) {
  if (command == fCmdVerbose) {
    fShift->SetVerboseLvl(fCmdVerbose->GetNewIntValue(newValues));
  } else if (command == fCmdPrintShiftTable) {
    fShift->PrintShiftTable();
  } else if (command == fCmdLoadShiftTable) {
    fShift->LoadShiftTable(newValues);
  } else if (command == fCmdSetMinShift) {
    fShift->SetMinShift(fCmdSetMinShift->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetMaxShift) {
    fShift->SetMaxShift(fCmdSetMaxShift->GetNewDoubleValue(newValues));
  } else if (command == fCmdConfineToMaterial) {
    fShift->ConfineToMaterial(newValues);
  }
}
