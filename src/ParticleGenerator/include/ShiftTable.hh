// Copyright [2024] C.Gruener
// Date: 24-06-16
// File: Shift table

#ifndef SRC_PARTICLEGENERATOR_INCLUDE_SHIFTTABLE_HH_
#define SRC_PARTICLEGENERATOR_INCLUDE_SHIFTTABLE_HH_

#include <string>
#include <vector>

#include "G4ThreeVector.hh"
#include "ParticleGenerator/include/ShiftTableMessenger.hh"
#include "Service/include/Logger.hh"

namespace Surface {
class Shift {
 public:
  explicit Shift(const G4int verboseLvl);
  explicit Shift(const G4String &filename, const G4int verboseLvl);
  ~Shift();
  void DoShift(G4ThreeVector &position, const G4ThreeVector &direction);
  void DoShiftByValue(const G4double shift, G4ThreeVector &position,
                      const G4ThreeVector &direction);
  std::stringstream StreamShiftTable() const;
  void PrintShiftTable();
  void LoadShiftTable(const std::string &filename);
  void SetMinShift(const G4double min);
  void SetMaxShift(const G4double max);
  void ConfineToMaterial(const G4String &materialName);
  void SetVerboseLvl(const G4int verboseLvl);

 private:
  G4double Interpolate(const G4int);
  G4double Interpolate(const G4double, const G4double, const G4double);
  G4double CalcShift();
  G4bool IsConfinedToMaterial(const G4ThreeVector &point);

 private:
  std::vector<G4double> fProbability;
  std::vector<G4double> fShift;
  std::vector<G4double> fBarProbability;
  G4bool fShiftTableReady;
  G4double fMinShift;
  G4double fMaxShift;
  G4String fConfineMaterialName;
  Logger fLogger;
  ShiftMessenger *fMessenger;
};
}  // namespace Surface
#endif  // SRC_PARTICLEGENERATOR_INCLUDE_SHIFTTABLE_HH_
