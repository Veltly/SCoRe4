// Copyright [2024] C.Gruener
// Date: 24-07-19
// File:

#include "Service/include/FileLogger.hh"

#include <cstdlib>
#include <iostream>

#include "G4ios.hh"

Surface::FileLogger::FileLogger(const G4String& filePath) noexcept
    : ss(std::stringstream()) {
  fFile.open(filePath);
}

Surface::FileLogger::~FileLogger() noexcept {
  if (fFile.is_open()) {
    fFile.close();
  }
}

void Surface::FileLogger::operator<<(const G4String& rhs) noexcept {
  this->ss << rhs;
}

void Surface::FileLogger::WriteLine() noexcept {
  if (fFile.is_open()) {
    fFile << ss.str() << "\n";
    ss.str("");
  } else {
    G4cout << "ERROR: File not open!" << G4endl;
    exit(EXIT_FAILURE);
  }
}

void Surface::FileLogger::WriteLine(const G4String& line) noexcept {
  this->ss << line;
  WriteLine();
}
