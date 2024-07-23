// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#ifndef SRC_SURFACEGENERATOR_INCLUDE_DESCRIBERMESSENGER_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_DESCRIBERMESSENGER_HH_

#include "G4UImessenger.hh"
#include "SurfaceGenerator/include/Describer.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

namespace Surface {

class DetectorConstruction;

class DescriberMessenger : public G4UImessenger {
 public:
  explicit DescriberMessenger(Surface::Describer *aDescriber);
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
  G4UIcmdWithAnInteger *fCmdSetNLayer;
};
}  // namespace Surface

#endif  // SRC_SURFACEGENERATOR_INCLUDE_DESCRIBERMESSENGER_HH_
