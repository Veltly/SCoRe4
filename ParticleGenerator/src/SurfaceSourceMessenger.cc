/*
 * SurfaceSourceMessenger.cc
 *
 *  Created on: Dec 4, 2022
 *      Author: cgruner
 */

#include "particleGenerators/SurfaceSourceMessenger.hh"

#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4Tokenizer.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

#include "G4StepLimiter.hh"
#include "particleGenerators/SurfaceSource.hh"

impCresst::particleGenerators::SurfaceSourceMessenger::SurfaceSourceMessenger(
	impCresst::particleGenerators::SurfaceSource *source) :
	Source(source), Directory(nullptr), CmdVerbose(nullptr), CmdShowContaminatedSurface(nullptr), CmdGenerateSurface(nullptr){
	//As the messenger class is instantiated by the PrimaryGeneratorSource, the pointer Source can not be NULL, hence no test needed.

	Directory = new G4UIdirectory("/surfacePlacement/");
	Directory->SetGuidance("Controls the particle source.");
	/** @todo Specify the state for which the commands are available. **/

	CmdVerbose = new G4UIcmdWithAnInteger("/surfacePlacement/verbose", this);
	CmdVerbose->SetParameterName("verbose", true); //omittable
	CmdVerbose->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
	CmdVerbose->SetGuidance("Set verbosity of the source.");

	CmdShowContaminatedSurface = new G4UIcmdWithABool("/surfacePlacement/ShowContaminatedSurface", this);
	CmdShowContaminatedSurface->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
	CmdShowContaminatedSurface->SetGuidance("Set if contaminated Surface should be drawn.");
	CmdShowContaminatedSurface->SetDefaultValue(false);

  CmdGenerateSurface = new G4UIcmdWithoutParameter("/surfacePlacement/generateSurface", this);
  CmdGenerateSurface->SetGuidance("Command to generate a Surface for particle placement");
  }

impCresst::particleGenerators::SurfaceSourceMessenger::~SurfaceSourceMessenger() {
	delete Directory; Directory = nullptr;
	delete CmdVerbose; CmdVerbose = nullptr;
  delete CmdGenerateSurface; CmdGenerateSurface = nullptr;
}

void impCresst::particleGenerators::SurfaceSourceMessenger::SetNewValue(G4UIcommand* command,
		G4String newValues) {
	if (command == CmdVerbose) {
		Source->SetVerboseLevel(CmdVerbose->GetNewIntValue(newValues));
  }else if (command == CmdGenerateSurface) {
    Source->GenerateSurface();
  }else if (command == CmdShowContaminatedSurface){
		Source->SetShowConaminatedSurface(CmdShowContaminatedSurface->GetNewBoolValue(newValues));
	}
}
