// Copyright [2024] C.Gruener
// Date: 24-07-26
// File:

#include "Service/include/MultiportalHelperMessenger.hh"

#include "G4ApplicationState.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "Service/include/MultiportalHelper.hh"

Surface::MultiportalHelperMessenger::MultiportalHelperMessenger(
    Surface::MultiportalHelper* source, const G4String& name)
    : fSource(source) {
  fDirectory = new G4UIdirectory("/Surface/");
  fDirectory->SetGuidance("Controls the MultiportalHelper.");
  fSubDirectory = new G4UIdirectory("/Surface/MultiportalHelper/");
  fSubDirectory->SetGuidance("Controls the MultiportalHelper.");
  const G4String ctrlPath = "/Surface/MultiportalHelper/" + name + "/";
  fHelperName = new G4UIdirectory(ctrlPath);

  const G4String cmdSetVerbose = ctrlPath + "setVerbose";
  fCmdSetVerbose = new G4UIcmdWithAnInteger(cmdSetVerbose, this);
  fCmdSetVerbose->AvailableForStates(G4State_PreInit, G4State_Init,
                                     G4State_Idle);
  fCmdSetVerbose->SetGuidance("Set VerboseLvl");

  const G4String cmdSetDxPortal = ctrlPath + "setDxPortal";
  fCmdSetDxPortal = new G4UIcmdWithADoubleAndUnit(cmdSetDxPortal, this);
  fCmdSetDxPortal->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetDxPortal->SetGuidance("Set portal size in x direction");

  const G4String cmdSetDyPortal = ctrlPath + "setDyPortal";
  fCmdSetDyPortal = new G4UIcmdWithADoubleAndUnit(cmdSetDyPortal, this);
  fCmdSetDyPortal->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetDyPortal->SetGuidance("Set portal size in y direction");

  const G4String cmdSetDzPortal = ctrlPath + "setDzPortal";
  fCmdSetDzPortal = new G4UIcmdWithADoubleAndUnit(cmdSetDzPortal, this);
  fCmdSetDzPortal->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetDzPortal->SetGuidance("Set portal size in z direction");

  const G4String cmdSetDxSubworld = ctrlPath + "setDxSubworld";
  fCmdSetDxSubworld = new G4UIcmdWithADoubleAndUnit(cmdSetDxSubworld, this);
  fCmdSetDxSubworld->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetDxSubworld->SetGuidance("Set subworld size in x direction");

  const G4String cmdSetDySubworld = ctrlPath + "setDySubworld";
  fCmdSetDySubworld = new G4UIcmdWithADoubleAndUnit(cmdSetDySubworld, this);
  fCmdSetDySubworld->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetDySubworld->SetGuidance("Set subworld size in y direction");

  const G4String cmdSetDzSubworld = ctrlPath + "setDzSubworld";
  fCmdSetDzSubworld = new G4UIcmdWithADoubleAndUnit(cmdSetDzSubworld, this);
  fCmdSetDzSubworld->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetDzSubworld->SetGuidance("Set subworld size in z direction");

  const G4String cmdSetSubworldMaterial = ctrlPath + "setMaterial";
  fCmdSetSubworldMaterial =
      new G4UIcmdWithAString(cmdSetSubworldMaterial, this);
  fCmdSetSubworldMaterial->AvailableForStates(G4State_PreInit, G4State_Init,
                                              G4State_Idle);
  fCmdSetSubworldMaterial->SetGuidance("Set material for subworld");

  const G4String cmdSetNxSubworld = ctrlPath + "setNxSubworld";
  fCmdSetNxSubworld = new G4UIcmdWithAnInteger(cmdSetNxSubworld, this);
  fCmdSetNxSubworld->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetNxSubworld->SetGuidance("Set number of subworlds in x direction");

  const G4String cmdSetNySubworld = ctrlPath + "setNySubworld";
  fCmdSetNySubworld = new G4UIcmdWithAnInteger(cmdSetNySubworld, this);
  fCmdSetNySubworld->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetNySubworld->SetGuidance("Set number of subworlds in y direction");
}

Surface::MultiportalHelperMessenger::~MultiportalHelperMessenger() {
  delete fDirectory;
  fDirectory = nullptr;
  delete fSubDirectory;
  fSubDirectory = nullptr;
  delete fHelperName;
  fHelperName = nullptr;

  delete fCmdSetVerbose;
  fCmdSetVerbose = nullptr;

  delete fCmdSetDxPortal;
  fCmdSetDxPortal = nullptr;
  delete fCmdSetDyPortal;
  fCmdSetDyPortal = nullptr;
  delete fCmdSetDzPortal;
  fCmdSetDzPortal = nullptr;

  delete fCmdSetDxSubworld;
  fCmdSetDxSubworld = nullptr;
  delete fCmdSetDySubworld;
  fCmdSetDySubworld = nullptr;
  delete fCmdSetDzSubworld;
  fCmdSetDzSubworld = nullptr;

  delete fCmdSetSubworldMaterial;
  fCmdSetSubworldMaterial = nullptr;

  delete fCmdSetNxSubworld;
  fCmdSetNxSubworld = nullptr;
  delete fCmdSetNySubworld;
  fCmdSetNySubworld = nullptr;
}

void Surface::MultiportalHelperMessenger::SetNewValue(G4UIcommand* command,
                                                      G4String newValues) {
  if (command == fCmdSetVerbose) {
    fSource->SetVerbose(fCmdSetVerbose->GetNewIntValue(newValues));
  } else if (command == fCmdSetDxPortal) {
    fSource->SetDxPortal(fCmdSetDxPortal->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetDyPortal) {
    fSource->SetDyPortal(fCmdSetDyPortal->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetDzPortal) {
    fSource->SetDzPortal(fCmdSetDzPortal->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetDxSubworld) {
    fSource->SetDxSub(fCmdSetDxSubworld->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetDySubworld) {
    fSource->SetDySub(fCmdSetDySubworld->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetDzSubworld) {
    fSource->SetDzSub(fCmdSetDzSubworld->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetSubworldMaterial) {
    fSource->SetSubworldMaterial(newValues);
  } else if (command == fCmdSetNxSubworld) {
    fSource->SetNxSub(fCmdSetNxSubworld->GetNewIntValue(newValues));
  } else if (command == fCmdSetNySubworld) {
    fSource->SetNySub(fCmdSetNySubworld->GetNewIntValue(newValues));
  }
}
