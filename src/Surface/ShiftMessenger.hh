/**
 * @brief Definition of class ShiftMessenger
 * @author C.Gruener
 * @date 2024-07-09
 * @file ShiftMessenger.hh
 */

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

namespace Surface {

class Shift;

/**
 * @brief Messenger for class Shift
 */
class ShiftMessenger : public G4UImessenger {

public:
  explicit ShiftMessenger(Surface::Shift *shift);
  ~ShiftMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValues) override;

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

#endif