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
#include <functional>

namespace Surface {
/**
 * @brief Definition of verbose level for logger class
 */
enum class VerboseLevel : G4int {
  Error = 0,
  Warning = 1,
  Info = 2,
  DetailInfo = 3,
  DebugInfo = 4,
  Default = 2
};

/**
 * @brief comparison operator for VerboseLevel
 * @param lhs
 * @param rhs
 * @return bool
 */
constexpr bool operator<=(VerboseLevel lhs,VerboseLevel rhs) {
  return static_cast<G4int>(lhs) <= static_cast<G4int>(rhs);
}

/// @brief The class handles generalized logging and the logging level
class Logger {
 public:
/// @brief constructor of Logger
/// @param aLoggerName Name of logger
/// @param aVerboseLvl logging level of logger
  explicit Logger(G4String aLoggerName, VerboseLevel aVerboseLvl = VerboseLevel::Default);
  explicit Logger(G4String aLoggerName, G4int aVerboseLvl);

 private:
  static VerboseLevel ToVerboseLvl(G4int aIntVerboseLvl) ;

 public:
  void WriteInfo(const G4String &) const;
  void WriteInfo(const G4String &&) const;
  void WriteInfo(const G4String &, const G4ThreeVector &) const;
  void WriteInfo(const G4String &&, const G4ThreeVector &) const;
  void WriteInfo(const std::stringstream &&) const;
  void WriteInfo(const std::function<std::string()>& string) const;

  void WriteDetailInfo(const G4String &) const;
  void WriteDetailInfo(const G4String &&) const;
  void WriteDetailInfo(const G4String &, const G4ThreeVector &) const;
  void WriteDetailInfo(const G4String &&, const G4ThreeVector &) const;
  void WriteDetailInfo(const std::stringstream &&) const;
  void WriteDetailInfo(const std::function<std::string()>& string) const;

  void WriteWarning(const G4String &) const;
  void WriteWarning(const G4String &&) const;
  void WriteWarning(const G4String &, const G4ThreeVector &) const;
  void WriteWarning(const G4String &&, const G4ThreeVector &) const;
  void WriteWarning(const std::stringstream &&) const;
  void WriteWarning(const std::function<std::string()>& string) const;

  void WriteError(const G4String &) const;
  void WriteError(const G4String &&) const;
  void WriteError(const G4String &, const G4ThreeVector &) const;
  void WriteError(const G4String &&, const G4ThreeVector &) const;
  void WriteError(const std::stringstream &&) const;
  void WriteError(const std::function<std::string()>& string) const;

  void WriteDebugInfo(const G4String &) const;
  void WriteDebugInfo(const G4String &&) const;
  void WriteDebugInfo(const G4String &, const G4ThreeVector &) const;
  void WriteDebugInfo(const G4String &&, const G4ThreeVector &) const;
  void WriteDebugInfo(const std::stringstream &&) const;
  void WriteDebugInfo(const std::function<std::string()>& string) const;

  // Setter
  void SetVerboseLvl(VerboseLevel aVerboseLvl);
  void SetVerboseLvl(G4int aIntVerboseLvl);

  // Getter
  inline VerboseLevel GetVerboseLvl() const { return fVerboseLvl; }
  inline const G4String &GetLoggerName() const { return fLoggerName; }
  static VerboseLevel GetDefaultVerboseLvl() {return VerboseLevel::Default;}

  inline G4bool IsErrorLvl() const {return fVerboseLvl <= VerboseLevel::Error;}
  inline G4bool IsWarningLvl() const {return fVerboseLvl <= VerboseLevel::Warning;}
  inline G4bool IsInfoLvl() const {return fVerboseLvl <= VerboseLevel::Info;}
  inline G4bool IsDetailInfoLvl() const {return fVerboseLvl <= VerboseLevel::DetailInfo;}
  inline G4bool IsDebugInfoLvl() const {return fVerboseLvl <= VerboseLevel::DebugInfo;}

 private:
  G4String fLoggerName; /// logger name of instance
  VerboseLevel fVerboseLvl; /// set verbose level of instance
};
}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_LOGGER_HH
