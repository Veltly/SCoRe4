/**
 * @details Implementation of Logger class
 * @file Logger.cc
 * @author C.Gruener
 * @date 23-06-01
 */

#include "Service/include/Logger.hh"

#include <G4ThreeVector.hh>
#include <G4ios.hh>
#include <utility>

Surface::Logger::Logger(G4String aLoggerName, const VerboseLevel aVerboseLvl)
    : fLoggerName(std::move(aLoggerName)), fVerboseLvl(aVerboseLvl) {}

Surface::Logger::Logger(G4String aLoggerName, G4int aVerboseLvl)
    : Logger(std::move(aLoggerName), ToVerboseLvl(aVerboseLvl)){}

Surface::VerboseLevel Surface::Logger::ToVerboseLvl(
    G4int aIntVerboseLvl) {
  if(aIntVerboseLvl < 0){
    return Surface::VerboseLevel::Error;
  }
  switch (aIntVerboseLvl){
    case 0: return Surface::VerboseLevel::Error;
    case 1: return Surface::VerboseLevel::Warning;
    case 2: return Surface::VerboseLevel::Info;
    case 3: return Surface::VerboseLevel::DetailInfo;
    default: return Surface::VerboseLevel::DebugInfo; // 4 or higher
  }
}

void Surface::Logger::SetVerboseLvl(const VerboseLevel aVerboseLvl) {
  fVerboseLvl = aVerboseLvl;
}

void Surface::Logger::SetVerboseLvl(G4int aIntVerboseLvl){
  SetVerboseLvl(ToVerboseLvl(aIntVerboseLvl));
}

// ----------------------------------------------
// -------------------- INFO --------------------
// ----------------------------------------------

void Surface::Logger::WriteInfo(const G4String &aMsg) const {
  if (IsInfoLvl()) {
    G4cout << " ----Info---------> " << fLoggerName << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteInfo(const G4String &&aMsg) const {
  WriteInfo(aMsg);
}

void Surface::Logger::WriteInfo(const G4String &aMsg,
                                      const G4ThreeVector &aVec) const {
  if (IsInfoLvl()) {
    G4cout << " ----Info---------> " << fLoggerName << ": " << aMsg
           << " X: " << aVec.x() << " Y: " << aVec.y() << " Z: " << aVec.z()
           << G4endl;
  }
}

void Surface::Logger::WriteInfo(const G4String &&aMsg, const G4ThreeVector &aVec) const {
  WriteInfo(aMsg,aVec);
}

void Surface::Logger::WriteInfo(const std::stringstream &&aMsg) const {
  WriteInfo(aMsg.str());
}

void Surface::Logger::WriteInfo(const std::function<std::string()> &string) const {
  if(IsInfoLvl()){
    G4cout << string() <<G4endl;
  }
}

// ----------------------------------------------
// ----------------- DETAIL INFO ----------------
// ----------------------------------------------

void Surface::Logger::WriteDetailInfo(const G4String &aMsg) const {
  if (IsDetailInfoLvl()) {
    G4cout << " ----DetailInfo---> " << fLoggerName << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteDetailInfo(const G4String &&aMsg) const {
  WriteDetailInfo(aMsg);
}

void Surface::Logger::WriteDetailInfo(const G4String &aMsg,
                                 const G4ThreeVector &aVec) const {
  if (IsDetailInfoLvl()) {
    G4cout << " ----DetailInfo---> " << fLoggerName << ": " << aMsg
           << " X: " << aVec.x() << " Y: " << aVec.y() << " Z: " << aVec.z()
           << G4endl;
  }
}

void Surface::Logger::WriteDetailInfo(const G4String &&aMsg, const G4ThreeVector &aVec) const {
  WriteDetailInfo(aMsg,aVec);
}

void Surface::Logger::WriteDetailInfo(const std::stringstream &&aMsg) const {
  WriteDetailInfo(aMsg.str());
}

void Surface::Logger::WriteDetailInfo(const std::function<std::string()> &string) const {
  if(IsDetailInfoLvl()){
    G4cout << string() <<G4endl;
  }
}

// ----------------------------------------------
// ------------------- WARNING ------------------
// ----------------------------------------------

void Surface::Logger::WriteWarning(const G4String &aMsg) const {
  if (IsWarningLvl()) {
    G4cout << " ----Warning------> " << fLoggerName << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteWarning(const G4String &&aMsg) const {
  WriteWarning(aMsg);
}

void Surface::Logger::WriteWarning(const G4String &aMsg,
                                 const G4ThreeVector &aVec) const {
  if (IsWarningLvl()) {
    G4cout << " ----Warning------> " << fLoggerName << ": " << aMsg
           << " X: " << aVec.x() << " Y: " << aVec.y() << " Z: " << aVec.z()
           << G4endl;
  }
}

void Surface::Logger::WriteWarning(const G4String &&aMsg,
                                 const G4ThreeVector &aVec) const {
  WriteWarning(aMsg, aVec);
}

void Surface::Logger::WriteWarning(const std::stringstream &&aMsg) const {
  WriteWarning(aMsg.str());
}

void Surface::Logger::WriteWarning(const std::function<std::string()> &string) const {
  if(IsWarningLvl()){
    G4cout << string() <<G4endl;
  }
}

// ----------------------------------------------
// -------------------- ERROR -------------------
// ----------------------------------------------

void Surface::Logger::WriteError(const G4String &aMsg) const {
  if (IsErrorLvl()) {
    G4cout << " ----Error--------> " << fLoggerName << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteError(const G4String &&aMsg) const {
  WriteError(aMsg);
}

void Surface::Logger::WriteError(const G4String &aMsg,
                                     const G4ThreeVector &aVec) const {
  if (IsErrorLvl()) {
    G4cout << " ----Error--------> " << fLoggerName << ": " << aMsg
           << " X: " << aVec.x() << " Y: " << aVec.y() << " Z: " << aVec.z()
           << G4endl;
  }
}

void Surface::Logger::WriteError(const G4String &&aMsg,
                                 const G4ThreeVector &aVec) const {
  WriteError(aMsg, aVec);
}

void Surface::Logger::WriteError(const std::stringstream &&aMsg) const {
  WriteError(aMsg.str());
}

void Surface::Logger::WriteError(const std::function<std::string()> &string) const {
  if(IsErrorLvl()){
    G4cout << string() <<G4endl;
  }
}

// ----------------------------------------------
// ----------------- DEBUG INFO -----------------
// ----------------------------------------------

void Surface::Logger::WriteDebugInfo(const G4String &aMsg) const {
  if (IsDebugInfoLvl()) {
    G4cout << " ----DebugInfo----> " << fLoggerName << ": " << aMsg << G4endl;
  }
}

void Surface::Logger::WriteDebugInfo(const G4String &&aMsg) const {
  WriteDebugInfo(aMsg);
}

void Surface::Logger::WriteDebugInfo(const G4String &aMsg,
                                     const G4ThreeVector &aVec) const {
  if (IsDebugInfoLvl()) {
    G4cout << " ----DebugInfo----> " << fLoggerName << ": " << aMsg
           << " X: " << aVec.x() << " Y: " << aVec.y() << " Z: " << aVec.z()
           << G4endl;
  }
}

void Surface::Logger::WriteDebugInfo(const G4String &&aMsg,
                                     const G4ThreeVector &aVec) const {
  WriteDebugInfo(aMsg, aVec);
}

void Surface::Logger::WriteDebugInfo(const std::stringstream &&stream) const {
  WriteDebugInfo(stream.str());
}

void Surface::Logger::WriteDebugInfo(const std::function<std::string()> &string) const {
  if(IsDebugInfoLvl()){
    G4cout << string() <<G4endl;
  }
}

// ----------------------------------------------
// ------------------- Always -------------------
// ----------------------------------------------

void Surface::Logger::WriteAlways(const G4String &aMsg) const {
    G4cout << " ----Info---------> " << fLoggerName << ": " << aMsg << G4endl;
}

void Surface::Logger::WriteAlways(const G4String &&aMsg) const {
  WriteAlways(aMsg);
}

void Surface::Logger::WriteAlways(const G4String &aMsg,
                                const G4ThreeVector &aVec) const {
    G4cout << " ----Info---------> " << fLoggerName << ": " << aMsg
           << " X: " << aVec.x() << " Y: " << aVec.y() << " Z: " << aVec.z()
           << G4endl;
}

void Surface::Logger::WriteAlways(const G4String &&aMsg, const G4ThreeVector &aVec) const {
  WriteAlways(aMsg,aVec);
}

void Surface::Logger::WriteAlways(const std::stringstream &&aMsg) const {
  WriteAlways(aMsg.str());
}

void Surface::Logger::WriteAlways(const std::function<std::string()> &string) const {
    G4cout << string() <<G4endl;
}