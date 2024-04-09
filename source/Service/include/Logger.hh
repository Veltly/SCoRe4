//
//
//
//
//	author:C.Gruener
//
//


#ifndef SURFACE_LOGGER
#define SURFACE_LOGGER
#include "G4String.hh"

namespace Surface {
class Logger {
public:
  Logger(G4String aId, G4int aVerboseLvl);
  void WriteInfo(G4String&);
  void WriteInfo(G4String&&);
  void WriteError(G4String&);
  void WriteError(G4String&&);
  void WriteDebugInfo(G4String&);
  void WriteDebugInfo(G4String&&);
  void SetVerboseLvl(G4int aVerboseLvl);
private:
  G4String fId;
  G4int fVerboseLvl;
};
} // namespace Surface
#endif
