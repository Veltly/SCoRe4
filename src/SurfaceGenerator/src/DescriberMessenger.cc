/**
 * @brief Functions to change Surface Generator settings via macro file
 * @author C.Gruener
 * @date 2023-06-01
 * @file DescriberMessenger.cc
 */

#include "SurfaceGenerator/include/DescriberMessenger.hh"

#include "G4ApplicationState.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include "SurfaceGenerator/include/Describer.hh"

Surface::DescriberMessenger::DescriberMessenger(Surface::Describer *aDescriber)
    : fDescriber(aDescriber),
      fDirectory(nullptr),
      fDirectory1(nullptr),
      fCmdSetSpikeWidth_X(nullptr),
      fCmdSetSpikeWidth_Y(nullptr),
      fCmdSetNrSpike_X(nullptr),
      fCmdSetNrSpike_Y(nullptr),
      fCmdSetMeanHeight(nullptr),
      fCmdSetHeightDeviation(nullptr),
      fCmdSetSpikeform(nullptr) {
  fDirectory = new G4UIdirectory("/geometry/");
  fDirectory->SetGuidance("Controls the implemented geometry.");
  fDirectory1 = new G4UIdirectory("/geometry/surface/");
  fDirectory1->SetGuidance("Controls the implemented surface geometry.");

  const G4String delimiter = " ";

  fCmdSetSpikeWidth_X =
      new G4UIcmdWithADoubleAndUnit("/geometry/surface/setSpikeWidth_X", this);
  fCmdSetSpikeWidth_X->AvailableForStates(G4State_PreInit, G4State_Init,
                                          G4State_Idle);
  fCmdSetSpikeWidth_X->SetGuidance(
      "Control width of spike in X direction (value is the half edge)");
  fCmdSetSpikeWidth_X->SetDefaultValue(0.005);
  fCmdSetSpikeWidth_X->SetDefaultUnit("mm");

  fCmdSetSpikeWidth_Y =
      new G4UIcmdWithADoubleAndUnit("/geometry/surface/setSpikeWidth_Y", this);
  fCmdSetSpikeWidth_Y->AvailableForStates(G4State_PreInit, G4State_Init,
                                          G4State_Idle);
  fCmdSetSpikeWidth_Y->SetGuidance(
      "Control width of spike in Y direction (value is the half edge)");
  fCmdSetSpikeWidth_Y->SetDefaultValue(0.005);
  fCmdSetSpikeWidth_Y->SetDefaultUnit("mm");

  fCmdSetNrSpike_X =
      new G4UIcmdWithAnInteger("/geometry/surface/setNrSpike_X", this);
  fCmdSetNrSpike_X->AvailableForStates(G4State_PreInit, G4State_Init,
                                       G4State_Idle);
  fCmdSetNrSpike_X->SetGuidance("Control the number of spikes in X direction");
  fCmdSetNrSpike_X->SetDefaultValue(1);

  fCmdSetNrSpike_Y =
      new G4UIcmdWithAnInteger("/geometry/surface/setNrSpike_Y", this);
  fCmdSetNrSpike_Y->AvailableForStates(G4State_PreInit, G4State_Init,
                                       G4State_Idle);
  fCmdSetNrSpike_X->SetGuidance("Control the number of spikes in Y direction");
  fCmdSetNrSpike_Y->SetDefaultValue(1);

  fCmdSetMeanHeight =
      new G4UIcmdWithADoubleAndUnit("/geometry/surface/setMeanHeight", this);
  fCmdSetMeanHeight->AvailableForStates(G4State_PreInit, G4State_Init,
                                        G4State_Idle);
  fCmdSetMeanHeight->SetGuidance(
      "Control the mean height of spikes. If no deviation of spikes i "
      "happening, all spikes have the mean height");
  fCmdSetMeanHeight->SetDefaultValue(0.005);
  fCmdSetMeanHeight->SetDefaultUnit("mm");

  fCmdSetHeightDeviation = new G4UIcmdWithADoubleAndUnit(
      "/geometry/surface/setHeightDeviation", this);
  fCmdSetHeightDeviation->AvailableForStates(G4State_PreInit, G4State_Init,
                                             G4State_Idle);
  fCmdSetHeightDeviation->SetGuidance(
      "Control the deviation from mean Height of spikes. Only used for "
      "specific spike forms");
  fCmdSetHeightDeviation->SetDefaultValue(1);

  fCmdSetSpikeform =
      new G4UIcmdWithAString("/geometry/surface/setSpikeform", this);
  fCmdSetSpikeform->AvailableForStates(G4State_PreInit, G4State_Init,
                                       G4State_Idle);
  fCmdSetSpikeform->SetGuidance(
      "Control the form of the spikes, all have similar form");
  fCmdSetSpikeform->SetDefaultValue("Standard");

  fCmdSetNLayer = new G4UIcmdWithAnInteger("/geometry/surface/setNLayer", this);
  fCmdSetNLayer->AvailableForStates(G4State_PreInit, G4State_Init,
                                    G4State_Idle);
  fCmdSetNLayer->SetGuidance("Control the number of layers in Z direction");
  fCmdSetNLayer->SetDefaultValue(1);
}

Surface::DescriberMessenger::~DescriberMessenger() {
  delete fCmdSetSpikeWidth_X;
  fCmdSetSpikeWidth_X = nullptr;
  delete fCmdSetSpikeWidth_Y;
  fCmdSetSpikeform = nullptr;
  delete fCmdSetNrSpike_X;
  fCmdSetNrSpike_X = nullptr;
  delete fCmdSetNrSpike_Y;
  fCmdSetNrSpike_Y = nullptr;
  delete fCmdSetMeanHeight;
  fCmdSetMeanHeight = nullptr;
  delete fCmdSetHeightDeviation;
  fCmdSetHeightDeviation = nullptr;
  delete fCmdSetSpikeform;
  fCmdSetSpikeform = nullptr;
  delete fCmdSetNLayer;
  fCmdSetNLayer = nullptr;
  delete fDirectory;
  fDirectory = nullptr;
  delete fDirectory1;
  fDirectory1 = nullptr;
  fDescriber = nullptr;  // No ownership, so don't delete it!
}

void Surface::DescriberMessenger::SetNewValue(G4UIcommand *command,
                                              G4String newValues) {
  if (command == fCmdSetSpikeWidth_X) {
    fDescriber->SetSpikeWidth_X(
        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValues));
  } else if (command == fCmdSetSpikeWidth_Y) {
    fDescriber->SetSpikeWidth_Y(
        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValues));
  } else if (command == fCmdSetNrSpike_X) {
    fDescriber->SetNrSpike_X(G4UIcmdWithAnInteger::GetNewIntValue(newValues));
  } else if (command == fCmdSetNrSpike_Y) {
    fDescriber->SetNrSpike_Y(G4UIcmdWithAnInteger::GetNewIntValue(newValues));
  } else if (command == fCmdSetMeanHeight) {
    fDescriber->SetMeanHeight(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValues));
  } else if (command == fCmdSetHeightDeviation) {
    fDescriber->SetHeightDeviation(
        G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValues));
  } else if (command == fCmdSetNLayer) {
    fDescriber->SetNLayer(G4UIcmdWithAnInteger::GetNewIntValue(newValues));
  } else if (command == fCmdSetSpikeform) {
    if (newValues == "StandardPyramid") {
      fDescriber->SetSpikeform(Surface::Describer::SpikeShape::StandardPyramid);
    } else if (newValues == "UniformPyramid") {
      fDescriber->SetSpikeform(Surface::Describer::SpikeShape::UniformPyramid);
    } else if (newValues == "Bump") {
      fDescriber->SetSpikeform(Surface::Describer::SpikeShape::Bump);
    } else if (newValues == "Peak") {
      fDescriber->SetSpikeform(Surface::Describer::SpikeShape::Peak);
    }
  }
}
