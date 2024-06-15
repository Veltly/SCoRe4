//
//
//
//
//	author:C.Gruener
//
//

#ifndef SURFACE_LOGGER
#define SURFACE_LOGGER
#include <G4String.hh>
#include <G4ThreeVector.hh>
namespace Surface {
///
/// Logger Class to standardize logging
///
class Logger {
public:
  Logger(G4String aId, G4int aVerboseLvl);
  void WriteInfo(G4String &);
  void WriteInfo(G4String &&);
  void WriteError(G4String &);
  void WriteError(G4String &&);
  void WriteDebugInfo(G4String &);
  void WriteDebugInfo(G4String &&);
  void WriteDebugInfo(G4String &, const G4ThreeVector &);
  void WriteDebugInfo(G4String &&, const G4ThreeVector &);
  void WriteDebugInfo(std::stringstream);
  void SetVerboseLvl(G4int aVerboseLvl);

private:
  G4String fId;
  G4int fVerboseLvl;
  G4int fVerboseError{0};
  G4int fVerboseInfo{1};
  G4int fVerboseDebugInfo{2};
};
} // namespace Surface
#endif
