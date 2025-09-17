/**
 * @brief Definition of class SurfaceSourceMessenger
 * @author C.Gruener
 * @date 2025-09-17
 * @file SurfaceSourceMessenger.hh
 */

#ifndef SURFACE_SURFACESOURCEMESSENGER
#define SURFACE_SURFACESOURCEMESSENGER

#include "G4String.hh"
#include "G4UImessenger.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;

namespace Surface {

class SurfaceSource;

//
//@brief Messenger for the Surface Source.
//
/**
 * @brief Messenger class for Surface Source
 */
class SurfaceSourceMessenger : public G4UImessenger {
 public:
  explicit SurfaceSourceMessenger(Surface::SurfaceSource *source);
  ~SurfaceSourceMessenger() override;

  void SetNewValue(G4UIcommand *command, G4String newValues) override;

 private:
  Surface::SurfaceSource *Source;
  G4UIdirectory *Directory;
  G4UIcmdWithAnInteger *CmdVerbose;
  G4UIcmdWithoutParameter *CmdShowSurface;
  G4UIcmdWithAString *CmdLogSurface;
};
}  // namespace Surface

#endif  // SURFACE_SURFACESOURCEMESSENGER
