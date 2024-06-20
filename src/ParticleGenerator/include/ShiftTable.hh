// Author: C.Gruener
// Date: 24-06-16
// File: Shift table

#ifndef SHIFTTABLE_HH
#define SHIFTTABLE_HH

#include "../../Service/include/Logger.hh"
#include "G4ThreeVector.hh"
#include <vector>

namespace Surface {
class Shift {
public:
  Shift(const std::string &filename, const G4int verbose = 3);
  ~Shift();
  void DoShift(G4ThreeVector &position, const G4ThreeVector &direction);
  void DoShiftByValue(const G4double shift, G4ThreeVector &position,
                      const G4ThreeVector &direction);
  void PrintShiftTable();

private:
  void LoadShiftTable(const std::string &filename);
  G4double Interpolate(const G4int);
  G4double Interpolate(const G4double, const G4double, const G4double);

private:
  std::vector<G4double> fProbability;
  std::vector<G4double> fShift;
  std::vector<G4double> fBarProbability;
  Logger fLogger;
};
} // namespace Surface
#endif // SHIFTTABLE_HH
