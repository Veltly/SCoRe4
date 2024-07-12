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
  G4bool WriteInfo();

  void WriteDetailInfo(G4String &);
  void WriteDetailInfo(G4String &&);
  void WriteDetailInfo(std::stringstream &&);
  G4bool WriteDetailInfo();

  void WriteWarning(G4String &);
  void WriteWarning(G4String &&);
  void WriteWarning(std::stringstream &&);
  G4bool WriteWarning();

  void WriteError(G4String &);
  void WriteError(G4String &&);
  void WriteError(std::stringstream &&);
  G4bool WriteError();

  void WriteDebugInfo(G4String &);
  void WriteDebugInfo(G4String &&);
  void WriteDebugInfo(G4String &, const G4ThreeVector &);
  void WriteDebugInfo(G4String &&, const G4ThreeVector &);
  void WriteDebugInfo(std::stringstream &&);
  G4bool WriteDebugInfo();

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
