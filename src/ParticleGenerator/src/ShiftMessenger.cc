/**
 * @brief Implementation of messenger class ShiftMessenger
 * @author C.Gruener
 * @date 2024-07-09
 * @file ShiftMessenger.cc
 */

#include "../include/ShiftMessenger.hh"

#include <G4ApplicationState.hh>

#include "../include/Shift.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

Surface::ShiftMessenger::ShiftMessenger(Surface::Shift *shift)
    : fShift(shift), fDirectory(nullptr), fCmdVerbose(nullptr),
      fCmdPrintShiftTable(nullptr), fCmdLoadShiftTable(nullptr),
      fCmdSetMinShift(nullptr), fCmdSetMaxShift(nullptr),
      fCmdConfineToMaterial(nullptr) {

  fDirectory = new G4UIdirectory("/shift/");
  fDirectory->SetGuidance("Controls the shift of the particle source");
  /** @todo Specify the state for which the commands are available. **/

  fCmdVerbose = new G4UIcmdWithAnInteger("/shift/verbose", this);
  fCmdVerbose->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
  fCmdVerbose->SetGuidance("Set verbose level of shift");

  fCmdPrintShiftTable =
      new G4UIcmdWithoutParameter("/shift/printShiftTable", this);
  fCmdPrintShiftTable->AvailableForStates(G4State_PreInit, G4State_Init,
                                          G4State_Idle);
  fCmdPrintShiftTable->SetGuidance("Print ShiftTable to output");

  fCmdLoadShiftTable = new G4UIcmdWithAString("/shift/loadShiftTable", this);
  fCmdLoadShiftTable->AvailableForStates(G4State_PreInit, G4State_Init,
                                         G4State_Idle);
  fCmdLoadShiftTable->SetGuidance("Load a ShiftTable from filepath");

  fCmdSetMinShift = new G4UIcmdWithADoubleAndUnit("/shift/setMinShift", this);
  fCmdSetMinShift->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetMinShift->SetGuidance("Set the minimal shift to do");
  fCmdSetMinShift->SetDefaultUnit("mm");
  fCmdSetMinShift->SetDefaultValue(0.);

  fCmdSetMaxShift = new G4UIcmdWithADoubleAndUnit("/shift/setMaxShift", this);
  fCmdSetMaxShift->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetMaxShift->SetGuidance("Set the maximal shift to do");
  fCmdSetMaxShift->SetDefaultUnit("mm");
  fCmdSetMaxShift->SetDefaultValue(DBL_MAX);

  fCmdConfineToMaterial =
      new G4UIcmdWithAString("/shift/confineToMaterial", this);
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
    fShift->SetVerboseLvl(G4UIcmdWithAnInteger::GetNewIntValue(newValues));
  } else if (command == fCmdPrintShiftTable) {
    fShift->PrintShiftTable();
  } else if (command == fCmdLoadShiftTable) {
    fShift->LoadShiftTable(newValues);
  } else if (command == fCmdSetMinShift) {
    fShift->SetMinShift(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValues));
  } else if (command == fCmdSetMaxShift) {
    fShift->SetMaxShift(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValues));
  } else if (command == fCmdConfineToMaterial) {
    fShift->ConfineToMaterial(newValues);
  }
}
