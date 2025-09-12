/**
 * @brief Messenger for RoughnessHelper class
 * @author C.Gruener
 * @date 2024-07-26
 * @file RoughnessHelperMessenger.hh
 */

#ifndef SRC_SERVICE_INCLUDE_ROUGHNESSHELPERMESSENGER_HH
#define SRC_SERVICE_INCLUDE_ROUGHNESSHELPERMESSENGER_HH

#include "G4String.hh"
#include "G4UImessenger.hh"
#include "Service/include/RoughnessHelper.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

namespace Surface {

class RoughnessHelper;
/**
 * @brief Messenger class for control of RoughnessHelper class via macro files
 */
class RoughnessHelperMessenger : public G4UImessenger {
 public:
  explicit RoughnessHelperMessenger(Surface::RoughnessHelper *source,
                                    const G4String &name);
  ~RoughnessHelperMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValues) override;

 private:
  Surface::RoughnessHelper *fSource;
  G4UIdirectory *fDirectory;
  G4UIdirectory *fSubDirectory;
  G4UIdirectory *fHelperName;

  G4UIcmdWithAnInteger *fCmdSetVerbose;

  G4UIcmdWithADoubleAndUnit *fCmdSetDxBasis;
  G4UIcmdWithADoubleAndUnit *fCmdSetDyBasis;
  G4UIcmdWithADoubleAndUnit *fCmdSetDzBasis;

  G4UIcmdWithADoubleAndUnit *fCmdSetDxSpike;
  G4UIcmdWithADoubleAndUnit *fCmdSetDySpike;
  G4UIcmdWithADoubleAndUnit *fCmdSetDzSpike;
  G4UIcmdWithADoubleAndUnit *fCmdSetDzDevSpike;
  G4UIcmdWithAString *fCmdSetSpikeform;
  G4UIcmdWithAnInteger *fCmdSetNLayer;

  G4UIcmdWithAnInteger *fCmdSetNxSpikes;
  G4UIcmdWithAnInteger *fCmdSetNySpikes;

  G4UIcmdWithAString *fCmdSetMaterial;

  G4UIcmdWithAnInteger *fCmdSetBoundaryNx;
  G4UIcmdWithAnInteger *fCmdSetBoundaryNy;
  G4UIcmdWithAnInteger *fCmdSetBoundaryNz;

  G4UIcmdWithADoubleAndUnit *fCmdSetStepLimit;
};
}  // namespace Surface

#endif  // SRC_SERVICE_INCLUDE_ROUGHNESSHELPERMESSENGER_HH
