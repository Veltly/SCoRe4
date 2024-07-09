// Author: C.Gruener
// Date: 24-06-16
// File: Shift table

#ifndef SHIFTTABLE_HH
#define SHIFTTABLE_HH

#include "../../Service/include/Logger.hh"
#include "../include/ShiftTableMessenger.hh"
#include "G4ThreeVector.hh"
#include <vector>

namespace Surface {
class Shift {
public:
  Shift(const G4int verbose = 3);
  ~Shift();
  void DoShift(G4ThreeVector &position, const G4ThreeVector &direction);
  void DoShiftByValue(const G4double shift, G4ThreeVector &position,
                      const G4ThreeVector &direction);
  void PrintShiftTable();
  void LoadShiftTable(const std::string &filename);
  void SetMinShift(G4double min);
  void SetMaxShift(G4double max);
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
  G4double fMinShift;
  G4double fMaxShift;
  G4String fConfineMaterialName;
  Logger fLogger;
  ShiftMessenger *fMessenger;
};
} // namespace Surface
#endif // SHIFTTABLE_HH
