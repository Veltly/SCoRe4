// Author: C.Gruener
// Date: 24-06-16
// File: ShiftTable

#include "../include/ShiftTable.hh"
#include "Randomize.hh"
#include <G4ThreeVector.hh>
#include <G4Types.hh>
#include <cstdlib>
#include <fstream>
#include <string>

Surface::Shift::Shift(const std::string &filename, const G4int verbose)
    : fLogger({"Shift", verbose}) {
  LoadShiftTable(filename);
};

Surface::Shift::~Shift(){};

void Surface::Shift::DoShift(G4ThreeVector &position,
                             const G4ThreeVector &direction) {
  G4double random = G4UniformRand();
  for (size_t i = 0; i < fBarProbability.size(); ++i) {
    if (random <= fBarProbability[i]) {
      const G4double shift = Interpolate(i);
      const G4ThreeVector normedDirection = direction / direction.r();
      position += normedDirection * shift;
      return;
    };
  }
}

void Surface::Shift::PrintShiftTable() {
  std::stringstream ss;
  ss << "Shift table:\n";
  for (size_t i = 0; i < fProbability.size(); ++i) {
    ss << "Shift: " << std::setw(10) << fShift[i]
       << " Counts: " << std::setw(10) << fProbability[i]
       << " Summed probability: " << std::setw(10) << fBarProbability[i]
       << "\n";
  }
  fLogger.WriteInfo(ss.str());
};

void Surface::Shift::LoadShiftTable(const std::string &filename) {
  std::ifstream file;
  file.open(filename);
  if (!file.is_open()) {
    fLogger.WriteError("File for shifttable not found!");
    return exit(EXIT_FAILURE);
  }
  std::string line;
  const std::string delimiter = ",";
  while (getline(file, line)) {
    const G4int pos = line.find(delimiter);
    const G4double depth = std::stod(line.substr(0, pos));
    const G4double counts = std::stod(line.substr(pos + 1, pos + line.size()));
    fShift.push_back(depth);
    fProbability.push_back(counts);
  }
  file.close();
  G4double counts{0};
  for (size_t i = 0; i < fProbability.size() - 1; ++i) {
    counts += Interpolate(0.5, fProbability.at(i), fProbability.at(i + 1));
  }

  G4double probability{0};
  for (size_t i = 0; i < fProbability.size() - 1; ++i) {
    probability +=
        Interpolate(0.5, fProbability.at(i), fProbability.at(i + 1)) / counts;
    fBarProbability.push_back(probability);
  }
};

G4double Surface::Shift::Interpolate(const G4double xNormed,
                                     const G4double lowerY,
                                     const G4double upperY) {
  const G4double k = (upperY - lowerY);
  const G4double result = lowerY + xNormed * k;
  return result;
};

G4double Surface::Shift::Interpolate(const G4int idx) {
  const G4double a = fProbability[idx];
  const G4double b = fProbability[idx + 1];
  const G4double rand = G4UniformRand();
  const G4double minShift = fShift[idx];
  const G4double maxShift = fShift[idx + 1];
  G4double tmp = sqrt(a * a * (1 - rand) + b * b * rand);
  tmp -= a;
  tmp /= (b - a);
  const G4double shift = minShift + tmp * (maxShift - minShift);
  // fLogger.WriteDebugInfo("prob[idx]: " + std::to_string(lowProb) +
  //                        " prob[idx + 1]: " + std::to_string(highProb) +
  //                        " minShift: " + std::to_string(minShift) +
  //                        " maxShift: " + std::to_string(maxShift) +
  //                        " rand: " + std::to_string(rand) +
  //                        " result: " + std::to_string(shift));
  return shift;
}
