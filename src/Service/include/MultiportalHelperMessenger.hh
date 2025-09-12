/**
 * @brief Messenger for MultiportalHelper class
 * @author C.Gruener
 * @date 2024-07-26
 * @file MultiportalHelperMessenger.hh
 */

#ifndef SRC_SERVICE_INCLUDE_MULTIPORTALHELPERMESSENGER_HH
#define SRC_SERVICE_INCLUDE_MULTIPORTALHELPERMESSENGER_HH

#include "G4String.hh"
#include "G4UImessenger.hh"
#include "Service/include/MultiportalHelper.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

namespace Surface {

class MultiportalHelper;

class MultiportalHelperMessenger final : public G4UImessenger {
 public:
  explicit MultiportalHelperMessenger(Surface::MultiportalHelper *source,
                                      const G4String &name);
  ~MultiportalHelperMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValues) override;

 private:
  Surface::MultiportalHelper *fSource;
  G4UIdirectory *fDirectory;
  G4UIdirectory *fSubDirectory;
  G4UIdirectory *fHelperName;

  G4UIcmdWithAnInteger *fCmdSetVerbose;

  G4UIcmdWithADoubleAndUnit *fCmdSetDxPortal;
  G4UIcmdWithADoubleAndUnit *fCmdSetDyPortal;
  G4UIcmdWithADoubleAndUnit *fCmdSetDzPortal;

  G4UIcmdWithADoubleAndUnit *fCmdSetDxSubworld;
  G4UIcmdWithADoubleAndUnit *fCmdSetDySubworld;
  G4UIcmdWithADoubleAndUnit *fCmdSetDzSubworld;

  G4UIcmdWithAString *fCmdSetSubworldMaterial;

  G4UIcmdWithAnInteger *fCmdSetNxSubworld;
  G4UIcmdWithAnInteger *fCmdSetNySubworld;
};
}  // namespace Surface

#endif  // SRC_SERVICE_INCLUDE_MULTIPORTALHELPERMESSENGER_HH