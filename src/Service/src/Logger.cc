// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#include "Service/include/Logger.hh"

#include <G4ThreeVector.hh>
#include <G4ios.hh>

Surface::Logger::Logger(G4String aId, G4int aVerboseLvl)
    : fId(aId), fVerboseLvl(aVerboseLvl) {}

void Surface::Logger::WriteInfo(const G4String &aMsg) const {
  if (fVerboseLvl > fVerboseInfo) {
    G4cout << " ----Info---------> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteInfo(const G4String &&aMsg) const {
  WriteInfo(aMsg);
}

void Surface::Logger::WriteInfo(const std::stringstream &&aMsg) const {
  WriteInfo(aMsg.str());
}

void Surface::Logger::WriteDetailInfo(const G4String &aMsg) const {
  if (fVerboseLvl > fVerboseDetailInfo) {
    G4cout << " ----Info---------> " << fId << ": " << aMsg << G4endl;
  }
}

G4bool Surface::Logger::WriteInfo() const { return fVerboseLvl > fVerboseInfo; }

void Surface::Logger::WriteDetailInfo(const G4String &&aMsg) const {
  WriteDetailInfo(aMsg);
}
void Surface::Logger::WriteDetailInfo(const std::stringstream &&aMsg) const {
  WriteDetailInfo(aMsg.str());
}

G4bool Surface::Logger::WriteDetailInfo() const {
  return fVerboseLvl > fVerboseDetailInfo;
}

void Surface::Logger::WriteWarning(const G4String &aMsg) const {
  if (fVerboseLvl > fVerboseWarning) {
    G4cout << " ----Warning------> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteWarning(const G4String &&aMsg) const {
  WriteWarning(aMsg);
}
void Surface::Logger::WriteWarning(const std::stringstream &&aMsg) const {
  WriteWarning(aMsg.str());
}

G4bool Surface::Logger::WriteWarning() const {
  return fVerboseLvl > fVerboseDetailInfo;
}

void Surface::Logger::WriteError(const G4String &aMsg) const {
  if (fVerboseLvl > fVerboseError) {
    G4cout << " ----Error--------> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteError(const G4String &&aMsg) const {
  WriteError(aMsg);
}

void Surface::Logger::WriteError(const std::stringstream &&aMsg) const {
  WriteError(aMsg.str());
}

G4bool Surface::Logger::WriteError() const {
  return fVerboseLvl > fVerboseDetailInfo;
}

void Surface::Logger::WriteDebugInfo(const G4String &aMsg) const {
  if (fVerboseLvl > fVerboseDebugInfo) {
    G4cout << " ----DebugInfo----> " << fId << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteDebugInfo(const G4String &&aMsg) const {
  WriteDebugInfo(aMsg);
}

void Surface::Logger::WriteDebugInfo(const G4String &&aMsg,
                                     const G4ThreeVector &aVec) const {
  WriteDebugInfo(aMsg, aVec);
}
void Surface::Logger::WriteDebugInfo(const G4String &aMsg,
                                     const G4ThreeVector &aVec) const {
  if (fVerboseLvl > fVerboseDebugInfo) {
    G4cout << " ----DebugInfo----> " << fId << ": " << aMsg
           << " X: " << aVec.x() << " Y: " << aVec.y() << " Z: " << aVec.z()
           << G4endl;
  }
}

void Surface::Logger::WriteDebugInfo(const std::stringstream &&stream) const {
  WriteDebugInfo(stream.str());
}

G4bool Surface::Logger::WriteDebugInfo() const {
  return fVerboseLvl > fVerboseDetailInfo;
}

void Surface::Logger::SetVerboseLvl(const G4int aVerboseLvl) {
  fVerboseLvl = aVerboseLvl;
}
