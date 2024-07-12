// Author: C.Gruener
// Date: 24-06-16
// File: ShiftTable

#include "../include/ShiftTable.hh"
#include "../include/ShiftTableMessenger.hh"
#include "Randomize.hh"
#include <G4Material.hh>
#include <G4SystemOfUnits.hh>
#include <G4ThreeVector.hh>
#include <G4TransportationManager.hh>
#include <G4Types.hh>
#include <G4VPhysicalVolume.hh>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <string>

Surface::Shift::Shift(const G4int verbose)
    : fShiftTableReady(false), fMinShift(0.), fMaxShift(DBL_MAX),
      fConfineMaterialName(""), fLogger({"Shift", verbose}),
      fMessenger(new Surface::ShiftMessenger(this)){};

Surface::Shift::Shift(const G4String &filename, const G4int verbose)
    : fShiftTableReady(false), fMinShift(0.), fMaxShift(DBL_MAX),
      fConfineMaterialName(""), fLogger({"Shift", verbose}),
      fMessenger(new Surface::ShiftMessenger(this)) {
  LoadShiftTable(filename);
};

Surface::Shift::~Shift(){};

G4double Surface::Shift::CalcShift() {
  G4double random = G4UniformRand();
  for (size_t i = 0; i < fBarProbability.size(); ++i) {
    if (random <= fBarProbability[i]) {
      return Interpolate(i) * CLHEP::nm;
    };
  }
}

void Surface::Shift::DoShift(G4ThreeVector &position,
                             const G4ThreeVector &direction) {
  if (not fShiftTableReady) {
    fLogger.WriteInfo("Shift called, but ShiftTable not ready\n"
                      "No shift done!");
    return;
  }

  G4int counter{0};
  while (true) {

    const G4double shift = CalcShift();

    const G4ThreeVector normedDirection = direction / direction.r();
    const G4ThreeVector newPosition = position - normedDirection * shift;

    ++counter;
    if (counter > 10000) {
      fLogger.WriteError("Counter of ShiftTable > 10,000! Now performing shift "
                         "with value outside marked area");
      position = newPosition;
      return;
    }

    if (fMinShift > shift or shift > fMaxShift) {
      continue;
    }

    if (not IsConfinedToMaterial(newPosition)) {
      continue;
    }
    fLogger.WriteDebugInfo("Shift done: " + std::to_string(shift));
    position = newPosition;
    return;
  }
}

void Surface::Shift::DoShiftByValue(const G4double shift,
                                    G4ThreeVector &position,
                                    const G4ThreeVector &direction) {
  const G4ThreeVector normedDirection = direction.unit();
  position -= normedDirection * shift;
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
    fLogger.WriteError("File for shifttable not found at: " + filename);
    return;
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
  fShiftTableReady = true;
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
  auto IsZero = [](const G4double a) {
    const G4double numeric_limit =
        std::numeric_limits<G4double>::epsilon() * 10;
    return fabs(a) < numeric_limit;
  };
  G4double tmp = 0;
  if (IsZero(b - a)) {
    tmp = rand;
  } else {
    tmp = sqrt(a * a * (1 - rand) + b * b * rand);
    tmp -= a;
    tmp /= (b - a);
  }
  const G4double shift = minShift + tmp * (maxShift - minShift);
  // fLogger.WriteDebugInfo("prob[idx]: " + std::to_string(lowProb) +
  //                        " prob[idx + 1]: " + std::to_string(highProb) +
  //                        " minShift: " + std::to_string(minShift) +
  //                        " maxShift: " + std::to_string(maxShift) +
  //                        " rand: " + std::to_string(rand) +
  //                        " result: " + std::to_string(shift));
  return shift;
}

void Surface::Shift::SetMinShift(G4double min) {
  fMinShift = min;
  fLogger.WriteInfo("Min shift set to " + std::to_string(min));
}
void Surface::Shift::SetMaxShift(G4double max) {
  fMaxShift = max;
  fLogger.WriteInfo("Max shift set to " + std::to_string(max));
}

G4bool Surface::Shift::IsConfinedToMaterial(const G4ThreeVector &point) {
  if (fConfineMaterialName == "") {
    return true;
  }
  G4ThreeVector null(0., 0., 0.);

  G4VPhysicalVolume *physVol =
      G4TransportationManager::GetTransportationManager()
          ->GetNavigatorForTracking()
          ->LocateGlobalPointAndSetup(point, &null, true);

  const G4String matName =
      physVol->GetLogicalVolume()->GetMaterial()->GetName();

  fLogger.WriteDebugInfo("Material Name of physical Volume is: " + matName);
  if (matName == fConfineMaterialName) {
    return true;
  }
  return false;
}

void Surface::Shift::ConfineToMaterial(const G4String &materialName) {
  fConfineMaterialName = materialName;
};

void Surface::Shift::SetVerboseLvl(const G4int verboseLvl) {
  fLogger.SetVerboseLvl(verboseLvl);
}
