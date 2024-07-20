// Copyright [2024] C.Gruener
// Date 24-07-19
// File:

#ifndef SRC_SERVICE_INCLUDE_FILELOGGER_HH_
#define SRC_SERVICE_INCLUDE_FILELOGGER_HH_

#include <fstream>
#include <sstream>

#include "G4String.hh"

namespace Surface {
///
/// Logger Class to standardize logging
///
class FileLogger {
 public:
  FileLogger(const G4String& filePath) noexcept;
  ~FileLogger() noexcept;

  void operator<<(const G4String& rhs) noexcept;

  void WriteLine() noexcept;
  void WriteLine(const G4String& line) noexcept;

 private:
  std::ofstream fFile;
  std::stringstream ss;
};
}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_FILELOGGER_HH_
