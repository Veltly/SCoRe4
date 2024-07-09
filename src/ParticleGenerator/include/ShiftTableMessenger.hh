// Author: C.Gruener
// Date: 24-07-09
// File: Shift Messenger
//

#ifndef SURFACE_SHIFTMESSENGER
#define SURFACE_SHIFTMESSENGER

#include "G4String.hh"
#include "G4UImessenger.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

namespace Surface {

class Shift;

//
//@Messenger for the class Shift
//

class ShiftMessenger : public G4UImessenger {

public:
  ShiftMessenger(Surface::Shift *shift);
  ~ShiftMessenger();

  void SetNewValue(G4UIcommand *command, G4String newValues);

private:
  Surface::Shift *fShift;
  G4UIdirectory *fDirectory;
  G4UIcmdWithAnInteger *fCmdVerbose;
  G4UIcmdWithoutParameter *fCmdPrintShiftTable;
  G4UIcmdWithAString *fCmdLoadShiftTable;
  G4UIcmdWithADoubleAndUnit *fCmdSetMinShift;
  G4UIcmdWithADoubleAndUnit *fCmdSetMaxShift;
  G4UIcmdWithAString *fCmdConfineToMaterial;
};
} // namespace Surface

#endif // SURFACE_SURFACESOURCEMESSENGER
