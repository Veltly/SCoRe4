// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#include "Service/include/Logger.hh"

#include <G4ThreeVector.hh>
#include <G4ios.hh>

Surface::Logger::Logger(G4String aId, G4int aVerboseLvl)
    : fId(aId), fVerboseLvl(aVerboseLvl) {}

void Surface::Logger::WriteInfo(G4String &aMsg) {
  if (fVerboseLvl > fVerboseInfo) {
    G4cout << " ----Info----> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteInfo(G4String &&aMsg) { WriteInfo(aMsg); }

void Surface::Logger::WriteInfo(std::stringstream &&aMsg) {
  WriteInfo(aMsg.str());
}

void Surface::Logger::WriteDetailInfo(G4String &aMsg) {
  if (fVerboseLvl > fVerboseDetailInfo) {
    G4cout << " ----Info----> " << fId << ": " << aMsg << G4endl;
  }
}

G4bool Surface::Logger::WriteInfo() { return fVerboseLvl > fVerboseInfo; }

void Surface::Logger::WriteDetailInfo(G4String &&aMsg) {
  WriteDetailInfo(aMsg);
}
void Surface::Logger::WriteDetailInfo(std::stringstream &&aMsg) {
  WriteDetailInfo(aMsg.str());
}

G4bool Surface::Logger::WriteDetailInfo() {
  return fVerboseLvl > fVerboseDetailInfo;
}

void Surface::Logger::WriteWarning(G4String &aMsg) {
  if (fVerboseLvl > fVerboseWarning) {
    G4cout << " ----Warning----> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteWarning(G4String &&aMsg) { WriteWarning(aMsg); }
void Surface::Logger::WriteWarning(std::stringstream &&aMsg) {
  WriteWarning(aMsg.str());
}

G4bool Surface::Logger::WriteWarning() {
  return fVerboseLvl > fVerboseDetailInfo;
}

void Surface::Logger::WriteError(G4String &aMsg) {
  if (fVerboseLvl > fVerboseError) {
    G4cout << " ----Error----> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteError(G4String &&aMsg) { WriteError(aMsg); }

void Surface::Logger::WriteError(std::stringstream &&aMsg) {
  WriteError(aMsg.str());
}

G4bool Surface::Logger::WriteError() {
  return fVerboseLvl > fVerboseDetailInfo;
}

void Surface::Logger::WriteDebugInfo(G4String &aMsg) {
  if (fVerboseLvl > fVerboseDebugInfo) {
    G4cout << " ----DebugInfo----> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteDebugInfo(G4String &&aMsg) { WriteDebugInfo(aMsg); }

void Surface::Logger::WriteDebugInfo(G4String &&aMsg,
                                     const G4ThreeVector &aVec) {
  WriteDebugInfo(aMsg, aVec);
}
void Surface::Logger::WriteDebugInfo(G4String &aMsg,
                                     const G4ThreeVector &aVec) {
  if (fVerboseLvl > fVerboseDebugInfo) {
    G4cout << " ----DebugInfo----> " << fId << ": " << aMsg
           << " X: " << aVec.x() << " Y: " << aVec.y() << " Z: " << aVec.z()
           << G4endl;
  }
}

void Surface::Logger::WriteDebugInfo(std::stringstream &&stream) {
  WriteDebugInfo(stream.str());
}

G4bool Surface::Logger::WriteDebugInfo() {
  return fVerboseLvl > fVerboseDetailInfo;
}

void Surface::Logger::SetVerboseLvl(G4int aVerboseLvl) {
  fVerboseLvl = aVerboseLvl;
}
