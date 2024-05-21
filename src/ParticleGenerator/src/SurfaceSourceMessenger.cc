//
//
//
//
//	author: C.Gruener
//
//

#include "../include/SurfaceSourceMessenger.hh"

#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4Tokenizer.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

#include "G4StepLimiter.hh"
#include "../include/SurfaceSource.hh"

Surface::SurfaceSourceMessenger::SurfaceSourceMessenger(
	Surface::SurfaceSource *source) :
	Source(source), Directory(nullptr), CmdVerbose(nullptr), CmdShowSurface(nullptr), CmdLogSurface(nullptr){
	//As the messenger class is instantiated by the PrimaryGeneratorSource, the pointer Source can not be NULL, hence no test needed.

	Directory = new G4UIdirectory("/SurfaceSource/");
	Directory->SetGuidance("Controls the particle source.");
	/** @todo Specify the state for which the commands are available. **/

	CmdShowSurface = new G4UIcmdWithoutParameter("/SurfaceSource/ShowSurface", this);
	CmdShowSurface->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
	CmdShowSurface->SetGuidance("Set if contaminated Surface should be drawn.");
	
	CmdLogSurface = new G4UIcmdWithAString("/SurfaceSource/ShowSurface", this);
	CmdLogSurface->AvailableForStates(G4State_PreInit, G4State_Init, G4State_Idle);
	CmdLogSurface->SetGuidance("Set if contaminated Surface should be writen to file.");
	CmdLogSurface->SetDefaultValue("SurfaceLog.csv");

  }

Surface::SurfaceSourceMessenger::~SurfaceSourceMessenger() {
	delete Directory;
	Directory = nullptr;
	delete CmdVerbose;
	CmdVerbose = nullptr;
	delete CmdShowSurface;
	CmdShowSurface = nullptr;
}

void Surface::SurfaceSourceMessenger::SetNewValue(G4UIcommand* command,
		G4String newValues) {
  if (command == CmdShowSurface){
		Source->ShowSurface();
	} else if (command == CmdLogSurface){
		Source->LogSurface(newValues);
	}
}
