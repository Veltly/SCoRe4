
#include "../include/Logger.hh"
#include <G4ios.hh>

Surface::Logger::Logger(G4String aId, G4int aVerboseLvl)
    : fId(aId), fVerboseLvl(aVerboseLvl){};

void Surface::Logger::WriteInfo(G4String &aMsg) {
  if (fVerboseLvl > fVerboseInfo) {
    ;
    G4cout << " ----Info----> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteInfo(G4String &&aMsg) { WriteInfo(aMsg); }

void Surface::Logger::WriteError(G4String &aMsg) {
  if (fVerboseLvl > fVerboseError) {
    G4cout << " ----Error----> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteError(G4String &&aMsg) { WriteError(aMsg); }

void Surface::Logger::WriteDebugInfo(G4String &aMsg) {
  if (fVerboseLvl > fVerboseDebugInfo) {
    G4cout << " ----DebugInfo----> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteDebugInfo(G4String &&aMsg) { WriteDebugInfo(aMsg); }

void Surface::Logger::WriteDebugInfo(std::stringstream stream) {
  WriteDebugInfo(stream.str());
}

void Surface::Logger::SetVerboseLvl(G4int aVerboseLvl) {
  fVerboseLvl = aVerboseLvl;
}
