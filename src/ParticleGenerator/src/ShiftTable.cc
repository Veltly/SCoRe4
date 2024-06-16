// Author: C.Gruener
// Date: 24-06-16
// File: ShiftTable

#include "../include/ShiftTable.hh"
#include "Randomize.hh"
#include <G4ThreeVector.hh>
#include <G4Types.hh>
#include <cstdlib>
#include <fstream>

Surface::Shift::Shift(const std::string &filename, const G4int verbose)
    : fLogger({"Shift", verbose}) {
  LoadShiftTable(filename);
};

Surface::Shift::~Shift(){};

void Surface::Shift::DoShift(G4ThreeVector &position,
                             const G4ThreeVector &direction) {
  G4double random = G4UniformRand();
  for (size_t i = 0; i < fProbability.size(); ++i) {
    if (random <= fProbability[i]) {
      const G4double shift = Interpolate(random, i);
      const G4ThreeVector normedDirection = direction / direction.SIZE;
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
       << " summed probability: " << std::setw(10) << fProbability[i] << "\n";
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
  std::map<G4double, G4double> fileData;
  const std::string delimiter = ",";
  while (getline(file, line)) {
    const G4int pos = line.find(delimiter);
    const G4double depth = std::stod(line.substr(0, pos));
    const G4double counts = std::stod(line.substr(pos + 1, pos + line.size()));
    fileData[depth] = counts;
  }
  file.close();
  G4double counts{0};
  for (auto &ele : fileData) {
    counts += ele.second;
  }
  G4double probability{0};
  for (auto &ele : fileData) {
    probability += ele.second / counts;
    fProbability.push_back(probability);
    fShift.push_back(ele.first);
  }
};

G4double Surface::Shift::Interpolate(const G4double random, const G4int idx) {
  const G4double prob = fProbability[idx - 1];
  const G4double diffProb = fProbability[idx] - prob;
  const G4double randomNormalized = (random - prob) / diffProb;
  const G4double shift = fShift[idx - 1];
  const G4double diffShift = fShift[idx] - shift;
  return shift + randomNormalized * diffShift;
};
