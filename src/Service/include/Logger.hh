// Copyright [2024] C.Gruener
// Date 23-06-01
// File:

#ifndef SRC_SERVICE_INCLUDE_LOGGER_HH_
#define SRC_SERVICE_INCLUDE_LOGGER_HH_

#include <sstream>

#include "G4String.hh"
#include "G4ThreeVector.hh"

namespace Surface {
///
/// Logger Class to standardize logging
///
class Logger {
 public:
  Logger(G4String aId, G4int aVerboseLvl = 4);

  void WriteInfo(const G4String &) const;
  void WriteInfo(const G4String &&) const;
  void WriteInfo(const std::stringstream &&) const;
  G4bool WriteInfo() const;

  void WriteDetailInfo(const G4String &) const;
  void WriteDetailInfo(const G4String &&) const;
  void WriteDetailInfo(const std::stringstream &&) const;
  G4bool WriteDetailInfo() const;

  void WriteWarning(const G4String &) const;
  void WriteWarning(const G4String &&) const;
  void WriteWarning(const std::stringstream &&) const;
  G4bool WriteWarning() const;

  void WriteError(const G4String &) const;
  void WriteError(const G4String &&) const;
  void WriteError(const std::stringstream &&) const;
  G4bool WriteError() const;

  void WriteDebugInfo(const G4String &) const;
  void WriteDebugInfo(const G4String &&) const;
  void WriteDebugInfo(const G4String &, const G4ThreeVector &) const;
  void WriteDebugInfo(const G4String &&, const G4ThreeVector &) const;
  void WriteDebugInfo(const std::stringstream &&) const;
  G4bool WriteDebugInfo() const;

  void SetVerboseLvl(const G4int aVerboseLvl);

 private:
  G4String fId;
  G4int fVerboseLvl;
  G4int fVerboseError{0};
  G4int fVerboseWarning{0};
  G4int fVerboseInfo{1};
  G4int fVerboseDetailInfo{2};
  G4int fVerboseDebugInfo{3};
};
}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_LOGGER_HH_
