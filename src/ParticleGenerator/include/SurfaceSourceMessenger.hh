//
//
//
//
//	author: C.Gruener
//
//

#ifndef SURFACE_SURFACESOURCEMESSENGER
#define SURFACE_SURFACESOURCEMESSENGER

#include "G4UImessenger.hh"
#include "G4String.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

namespace Surface {

class SurfaceSource;

//
//@brief Messenger for the Surface Source.
//

class SurfaceSourceMessenger: public G4UImessenger {

public:
	SurfaceSourceMessenger(Surface::SurfaceSource *source);
	~SurfaceSourceMessenger();

	void SetNewValue(G4UIcommand *command, G4String newValues);

private:
	Surface::SurfaceSource *Source;
	G4UIdirectory *Directory;
	G4UIcmdWithAnInteger *CmdVerbose;
	G4UIcmdWithoutParameter *CmdShowSurface;
	G4UIcmdWithAString *CmdLogSurface;
};
}

#endif // SURFACE_SURFACESOURCEMESSENGER
