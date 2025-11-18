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

class PointShift;

/**
 * @brief Messenger for class Shift
 */
class PointShiftMessenger : public G4UImessenger {

public:
  explicit PointShiftMessenger(Surface::PointShift *shift);
  ~PointShiftMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValues) override;

private:
  Surface::PointShift *fShift;
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