//
//
//
//
//  author: C.Gruener
//
//

#include "../include/DescriberMessenger.hh"
#include "../include/Describer.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include <G4ApplicationState.hh>

Surface::DescriberMessenger::DescriberMessenger(Surface::Describer *aDescriber)
    : fDescriber(aDescriber), fDirectory(NULL), fDirectory1(NULL),
      fCmdSetSpikeWidth_X(NULL), fCmdSetSpikeWidth_Y(NULL),
      fCmdSetNrSpike_X(NULL), fCmdSetNrSpike_Y(NULL), fCmdSetMeanHeight(NULL),
      fCmdSetHeightDeviation(NULL), fCmdSetSpikeform(NULL) {
  fDirectory = new G4UIdirectory("/geometry/");
  fDirectory->SetGuidance("Controls the implemented geometry.");
  fDirectory1 = new G4UIdirectory("/geometry/surface/");
  fDirectory1->SetGuidance("Controls the implemented surface geometry.");

  G4String delimiter = " ";

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
      "specific spikeforms");
  fCmdSetHeightDeviation->SetDefaultValue(1);

  fCmdSetSpikeform =
      new G4UIcmdWithAString("/geometry/surface/setSpikeform", this);
  fCmdSetSpikeform->AvailableForStates(G4State_PreInit, G4State_Init,
                                       G4State_Idle);
  fCmdSetSpikeform->SetGuidance(
      "Control the form of the spikes, all have similar form");
  fCmdSetSpikeform->SetDefaultValue("Standard");
}

Surface::DescriberMessenger::~DescriberMessenger() {
  delete fCmdSetSpikeWidth_X;
  fCmdSetSpikeWidth_X = NULL;
  delete fCmdSetSpikeWidth_Y;
  fCmdSetSpikeform = NULL;
  delete fCmdSetNrSpike_X;
  fCmdSetNrSpike_X = NULL;
  delete fCmdSetNrSpike_Y;
  fCmdSetNrSpike_Y = NULL;
  delete fCmdSetMeanHeight;
  fCmdSetMeanHeight = NULL;
  delete fCmdSetHeightDeviation;
  fCmdSetHeightDeviation = NULL;
  delete fCmdSetSpikeform;
  fCmdSetSpikeform = NULL;
  delete fDirectory;
  fDirectory = NULL;
  delete fDirectory1;
  fDirectory1 = NULL;
  fDescriber = NULL; // No ownership, so don't delete it!
}

void Surface::DescriberMessenger::SetNewValue(G4UIcommand *command,
                                              G4String newValues) {
  if (command == fCmdSetSpikeWidth_X) {
    fDescriber->SetSpikeWidth_X(
        fCmdSetSpikeWidth_X->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetSpikeWidth_Y) {
    fDescriber->SetSpikeWidth_Y(
        fCmdSetSpikeWidth_Y->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetNrSpike_X) {
    fDescriber->SetNrSpike_X(fCmdSetNrSpike_X->GetNewIntValue(newValues));
  } else if (command == fCmdSetNrSpike_Y) {
    fDescriber->SetNrSpike_Y(fCmdSetNrSpike_Y->GetNewIntValue(newValues));
  } else if (command == fCmdSetMeanHeight) {
    fDescriber->SetMeanHeight(fCmdSetMeanHeight->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetHeightDeviation) {
    fDescriber->SetHeightDeviation(
        fCmdSetHeightDeviation->GetNewDoubleValue(newValues));
  } else if (command == fCmdSetSpikeform) {
    if (newValues == "StandardPyramide") {
      fDescriber->SetSpikeform(Surface::Describer::Spikeform::StandardPyramide);
    } else if (newValues == "UniformPyramide") {
      fDescriber->SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);
    } else if (newValues == "Bump") {
      fDescriber->SetSpikeform(Surface::Describer::Spikeform::Bump);
    } else if (newValues == "Peak") {
      fDescriber->SetSpikeform(Surface::Describer::Spikeform::Peak);
    }
  }
}
