/*
 * SurfaceSourceMessenger.hh
 *
 *  Created on: Dec 4, 2022
 *      Author: cgruner
 */

#ifndef INCLUDE_SURFACESOURCEMESSENGER_HH_
#define INCLUDE_SURFACESOURCEMESSENGER_HH_

#include "G4UImessenger.hh"
#include "G4String.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

namespace impCresst {
namespace particleGenerators {

class SurfaceSource;

/**
 * @brief Messenger for the Surface Source.
 */
class SurfaceSourceMessenger: public G4UImessenger {

public:
	SurfaceSourceMessenger(impCresst::particleGenerators::SurfaceSource *source);
	~SurfaceSourceMessenger();

	void SetNewValue(G4UIcommand *command, G4String newValues);

private:
	impCresst::particleGenerators::SurfaceSource *Source;
	G4UIdirectory *Directory;
	G4UIcmdWithAnInteger *CmdVerbose;
	G4UIcmdWithABool *CmdShowContaminatedSurface;
  G4UIcmdWithoutParameter *CmdGenerateSurface;
};
}
}

#endif /* INCLUDE_SURFACESOURCEMESSENGER_HH_ */
