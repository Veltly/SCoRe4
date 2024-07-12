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
#include <sstream>
namespace Surface {
///
/// Logger Class to standardize logging
///
class Logger {
public:
  Logger(G4String aId, G4int aVerboseLvl);
  void WriteInfo(G4String &);
  void WriteInfo(G4String &&);
  void WriteInfo(std::stringstream &&);
  void WriteInfo(void (*print)());

  void WriteDetailInfo(G4String &);
  void WriteDetailInfo(G4String &&);
  void WriteDetailInfo(std::stringstream &&);
  void WriteDetailInfo(void (*print)());

  void WriteWarning(G4String &);
  void WriteWarning(G4String &&);
  void WriteWarning(std::stringstream &&);
  void WriteWarning(void (*print)());

  void WriteError(G4String &);
  void WriteError(G4String &&);
  void WriteError(std::stringstream &&);
  void WriteError(void (*print)());

  void WriteDebugInfo(G4String &);
  void WriteDebugInfo(G4String &&);
  void WriteDebugInfo(G4String &, const G4ThreeVector &);
  void WriteDebugInfo(G4String &&, const G4ThreeVector &);
  void WriteDebugInfo(std::stringstream &&);
  void WriteDebugInfo(void (*print)());

  void SetVerboseLvl(G4int aVerboseLvl);

private:
  G4String fId;
  G4int fVerboseLvl;
  G4int fVerboseError{0};
  G4int fVerboseWarning{0};
  G4int fVerboseInfo{1};
  G4int fVerboseDetailInfo{2};
  G4int fVerboseDebugInfo{3};
};
} // namespace Surface
#endif
