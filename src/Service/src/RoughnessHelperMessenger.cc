// Copyright [2024] C.Gruener
// Date: 24-07-26
// File:

#include "Service/include/RoughnessHelperMessenger.hh"

#include "G4ApplicationState.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "Service/include/RoughnessHelper.hh"

Surface::RoughnessHelperMessenger::RoughnessHelperMessenger(
    Surface::RoughnessHelper* source)
    : fSource(source) {
  fDirectory = new G4UIdirectory("/Surface/");
  fDirectory->SetGuidance("Controls the RoughnessHelper.");
  fSubDirectory = new G4UIdirectory("/Surface/RoughnessHelper");
  fSubDirectory->SetGuidance("Controls the RoughnessHelper.");
  const G4String ctrlPath =
      "/Surface/RoughnessHelper/" + fSource->GetName() + "/";
  fHelperName = new G4UIdirectory(ctrlPath);

  const G4String cmdSetVerbose = ctrlPath + "setVerbose";
  fCmdSetVerbose = new G4UIcmdWithAnInteger(cmdSetVerbose, this);
  fCmdSetVerbose->AvailableForStates(G4State_PreInit, G4State_Init,
                                     G4State_Idle);
  fCmdSetVerbose->SetGuidance("Set VerboseLvl");

  const G4String cmdSetDxBasis = ctrlPath + "setBasisDx";
  fCmdSetDxBasis = new G4UIcmdWithADoubleAndUnit(cmdSetDxBasis, this);
  fCmdSetDxBasis->AvailableForStates(G4State_PreInit, G4State_Init,
                                     G4State_Idle);
  fCmdSetDxBasis->SetGuidance("Set portal size in x direction");

  const G4String cmdSetDyBasis = ctrlPath + "setBasisDy";
  fCmdSetDyBasis = new G4UIcmdWithADoubleAndUnit(cmdSetDyBasis, this);
  fCmdSetDyBasis->AvailableForStates(G4State_PreInit, G4State_Init,
                                     G4State_Idle);
  fCmdSetDyBasis->SetGuidance("Set portal size in y direction");

  const G4String cmdSetDzBasis = ctrlPath + "setBasisDz";
  fCmdSetDzBasis = new G4UIcmdWithADoubleAndUnit(cmdSetDzBasis, this);
  fCmdSetDzBasis->AvailableForStates(G4State_PreInit, G4State_Init,
                                     G4State_Idle);
  fCmdSetDzBasis->SetGuidance("Set portal size in z direction");

  const G4String cmdSetDxSpike = ctrlPath + "setSpikeDx";
  fCmdSetDxSpike = new G4UIcmdWithADoubleAndUnit(cmdSetDxSpike, this);
  fCmdSetDxSpike->AvailableForStates(G4State_PreInit, G4State_Init,
                                     G4State_Idle);
  fCmdSetDxSpike->SetGuidance("Set subworld size in x direction");

  const G4String cmdSetDySpike = ctrlPath + "setSpikeDy";
  fCmdSetDySpike = new G4UIcmdWithADoubleAndUnit(cmdSetDySpike, this);
  fCmdSetDySpike->AvailableForStates(G4State_PreInit, G4State_Init,
                                     G4State_Idle);
  fCmdSetDySpike->SetGuidance("Set subworld size in y direction");

  const G4String cmdSetDzSpike = ctrlPath + "setSpikeMeanHeight";
  fCmdSetDzSpike = new G4UIcmdWithADoubleAndUnit(cmdSetDzSpike, this);
  fCmdSetDzSpike->AvailableForStates(G4State_PreInit, G4State_Init,
                                     G4State_Idle);
  fCmdSetDzSpike->SetGuidance("Set spike height in z direction");

  const G4String cmdSetMaterial = ctrlPath + "setMaterial";
  fCmdSetMaterial = new G4UIcmdWithAString(cmdSetMaterial, this);
  fCmdSetMaterial->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetMaterial->SetGuidance("Set material for subworld");

  const G4String cmdSetNxSpikes = ctrlPath + "setSpikesNx";
  fCmdSetNxSpikes = new G4UIcmdWithAnInteger(cmdSetNxSpikes, this);
  fCmdSetNxSpikes->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetNxSpikes->SetGuidance("Set number of subworlds in x direction");

  const G4String cmdSetNySpikes = ctrlPath + "setSpikesNy";
  fCmdSetNySpikes = new G4UIcmdWithAnInteger(cmdSetNySpikes, this);
  fCmdSetNySpikes->AvailableForStates(G4State_PreInit, G4State_Init,
                                      G4State_Idle);
  fCmdSetNySpikes->SetGuidance("Set number of subworlds in y direction");

  const G4String cmdSetBoundaryNx = ctrlPath + "setBoundaryNx";
  fCmdSetBoundaryNx = new G4UIcmdWithAnInteger(cmdSetBoundaryNx, this);
  fCmdSetBoundaryNx->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetBoundaryNx->SetGuidance("Set number of boundaries in x direction");

  const G4String cmdSetBoundaryNy = ctrlPath + "setBoundaryNy";
  fCmdSetBoundaryNy = new G4UIcmdWithAnInteger(cmdSetBoundaryNy, this);
  fCmdSetBoundaryNy->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetBoundaryNy->SetGuidance("Set number of boundaries in y direction");

  const G4String cmdSetBoundaryNz = ctrlPath + "setBoundaryNz";
  fCmdSetBoundaryNz = new G4UIcmdWithAnInteger(cmdSetBoundaryNz, this);
  fCmdSetBoundaryNz->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetBoundaryNz->SetGuidance("Set number of boundaries in z direction");

  const G4String cmdSetStepLimit = ctrlPath + "setStepLimit";
  fCmdSetStepLimit = new G4UIcmdWithADoubleAndUnit(cmdSetStepLimit, this);
  fCmdSetStepLimit->AvailableForStates(G4State_PreInit, G4State_Init,
                                       G4State_Idle);
  fCmdSetStepLimit->SetGuidance("Set steplimit for roughness");
}

Surface::RoughnessHelperMessenger::~RoughnessHelperMessenger() {
  delete fDirectory;
  fDirectory = nullptr;
  delete fSubDirectory;
  fSubDirectory = nullptr;
  delete fHelperName;
  fHelperName = nullptr;

  delete fCmdSetVerbose;
  fCmdSetVerbose = nullptr;

  delete fCmdSetDxBasis;
  fCmdSetDxBasis = nullptr;
  delete fCmdSetDyBasis;
  fCmdSetDyBasis = nullptr;
  delete fCmdSetDzBasis;
  fCmdSetDzBasis = nullptr;

  delete fCmdSetDxSpike;
  fCmdSetDxSpike = nullptr;
  delete fCmdSetDySpike;
  fCmdSetDySpike = nullptr;
  delete fCmdSetDzSpike;
  fCmdSetDzSpike = nullptr;

  delete fCmdSetNxSpikes;
  fCmdSetNxSpikes = nullptr;
  delete fCmdSetNySpikes;
  fCmdSetNySpikes = nullptr;

  delete fCmdSetMaterial;
  fCmdSetMaterial = nullptr;

  delete fCmdSetBoundaryNx;
  fCmdSetBoundaryNx = nullptr;
  delete fCmdSetBoundaryNy;
  fCmdSetBoundaryNy = nullptr;
  delete fCmdSetBoundaryNz;
  fCmdSetBoundaryNz = nullptr;

  delete fCmdSetStepLimit;
  fCmdSetStepLimit = nullptr;
}

void Surface::RoughnessHelperMessenger::SetNewValue(G4UIcommand* command,
                                                    G4String newValues) {
  if (command == fCmdSetVerbose) {
    fSource->SetVerbose(fCmdSetVerbose->GetNewIntValue(newValues));
  } else if (command == fCmdSetDxBasis) {
    fSource->SetBasisDx(fCmdSetDxBasis->GetNewUnitValue(newValues));
  } else if (command == fCmdSetDyBasis) {
    fSource->SetBasisDy(fCmdSetDyBasis->GetNewUnitValue(newValues));
  } else if (command == fCmdSetDzBasis) {
    fSource->SetBasisHeight(fCmdSetDzBasis->GetNewUnitValue(newValues));
  } else if (command == fCmdSetDxSpike) {
    fSource->SetSpikeDx(fCmdSetDxSpike->GetNewUnitValue(newValues));
  } else if (command == fCmdSetDySpike) {
    fSource->SetSpikeDy(fCmdSetDySpike->GetNewUnitValue(newValues));
  } else if (command == fCmdSetDzSpike) {
    fSource->SetSpikeMeanHeight(fCmdSetDzSpike->GetNewUnitValue(newValues));
  } else if (command == fCmdSetNLayer) {
    fSource->SetSpikeNLayer(fCmdSetNLayer->GetNewIntValue(newValues));
  } else if (command == fCmdSetNxSpikes) {
    fSource->SetSpikeNx(fCmdSetNxSpikes->GetNewIntValue(newValues));
  } else if (command == fCmdSetNySpikes) {
    fSource->SetSpikeNy(fCmdSetNySpikes->GetNewIntValue(newValues));
  } else if (command == fCmdSetMaterial) {
    fSource->SetMaterial(newValues);
  } else if (command == fCmdSetBoundaryNx) {
    fSource->SetBoundaryX(fCmdSetBoundaryNx->GetNewIntValue(newValues));
  } else if (command == fCmdSetBoundaryNy) {
    fSource->SetBoundaryY(fCmdSetBoundaryNy->GetNewIntValue(newValues));
  } else if (command == fCmdSetBoundaryNz) {
    fSource->SetBoundaryZ(fCmdSetBoundaryNz->GetNewIntValue(newValues));
  } else if (command == fCmdSetStepLimit) {
    fSource->SetStepLimit(fCmdSetStepLimit->GetNewUnitValue(newValues));
  }
}
