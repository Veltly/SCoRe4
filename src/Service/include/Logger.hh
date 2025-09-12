/**
 * @file Logger.hh
 * @author C.Gruener
 * @brief The class "Logger" handles generalized logging
 * @date 2023-06-01
 */

#ifndef SRC_SERVICE_INCLUDE_LOGGER_HH
#define SRC_SERVICE_INCLUDE_LOGGER_HH

#include "G4String.hh"
#include "G4ThreeVector.hh"

namespace Surface {
/// @brief The class handles generalized logging and the logging level
class Logger {
 public:
/// @brief constructor of Logger
/// @param aLoggerName Name of logger
/// @param aVerboseLvl logging level of logger
  explicit Logger(G4String aLoggerName, G4int aVerboseLvl = fgDefaultVerboseLvl);

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

  // Setter
  void SetVerboseLvl(G4int aVerboseLvl);

  // Getter
  inline G4int GetVerboseLvl() const { return fVerboseLvl; }
  inline const G4String &GetLoggerName() { return fLoggerName; }
  static G4int GetDefaultVerboseLvl() {return fgDefaultVerboseLvl;}

 private:
  G4String fLoggerName; /// logger name of instance
  G4int fVerboseLvl; /// set verbose level of instance
  G4int fVerboseError{0}; /// verbose level of error
  G4int fVerboseWarning{0}; /// verbose level of warning
  G4int fVerboseInfo{1}; /// verbose level of info
  G4int fVerboseDetailInfo{2}; /// verbose level of detailed info
  G4int fVerboseDebugInfo{3}; /// verbose level of debug info

  static const G4int fgDefaultVerboseLvl{4};
};
}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_LOGGER_HH
