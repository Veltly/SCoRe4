//
//
//
//
//	author: C.Gruener
//
//

#ifndef SURFACE_DESCRIBERMESSENGER
#define SURFACE_DESCRIBERMESSENGER

#include "G4UImessenger.hh"
#include "Describer.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
namespace Surface{

class DetectorConstruction;

class DescriberMessenger: public G4UImessenger {

public:
	DescriberMessenger(Surface::Describer *aDescriber);
	virtual ~DescriberMessenger();

	void SetNewValue(G4UIcommand *command, G4String newValues);

private:
	Surface::Describer *fDescriber;
	G4UIdirectory *fDirectory;
    	G4UIdirectory *fDirectory1;
	
	G4UIcmdWithADoubleAndUnit *fCmdSetSpikeWidth_X;
	G4UIcmdWithADoubleAndUnit *fCmdSetSpikeWidth_Y;
	G4UIcmdWithAnInteger *fCmdSetNrSpike_X;
	G4UIcmdWithAnInteger *fCmdSetNrSpike_Y;
	G4UIcmdWithADoubleAndUnit *fCmdSetMeanHeight;
	G4UIcmdWithADoubleAndUnit *fCmdSetHeightDeviation;
	G4UIcmdWithAString *fCmdSetSpikeform;
};
}//namespace: Surface

#endif //SURFACE__DESCRIBERMESSENGER
