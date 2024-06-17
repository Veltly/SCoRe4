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
  G4double val{0};
  if (idx - 2 >= 0) {
    val = fProbability[idx - 2];
  }
  G4double lowProb{0};
  if (idx - 1 >= 0) {
    lowProb = fProbability[idx - 1];
  }
  //  fLogger.WriteDebugInfo("IDX: " + std::to_string(idx));
  const G4double randNorm = (random - lowProb) / (fProbability[idx] - lowProb);
  const G4double a = fProbability[idx] - lowProb;
  const G4double b = fProbability[idx + 1] - fProbability[idx];
  const G4double v = fShift[idx];
  const G4double w = fShift[idx + 1];
  G4double tmp = sqrt(a * a * (1 - randNorm) + b * b * randNorm);
  tmp -= a;
  tmp /= (b - a);
  const G4double shift = v + tmp * (w - v);
  //  fLogger.WriteDebugInfo(
  //      "prob[idx - 1]: " + std::to_string(lowProb) + " prob[idx]: " +
  //      std::to_string(fProbability[idx]) + "a: " + std::to_string(a) +
  //      " b: " + std::to_string(b) + " v: " + std::to_string(v) +
  //      " w: " + std::to_string(w) + " result: " + std::to_string(shift));
  return shift;
};
