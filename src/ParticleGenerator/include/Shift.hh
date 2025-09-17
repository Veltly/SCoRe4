/**
 * @brief Definition of class Shift
 * @author C.Gruener
 * @date 2024-06-16
 * @file Shift.hh
 */

#ifndef SRC_PARTICLEGENERATOR_INCLUDE_SHIFT_HH
#define SRC_PARTICLEGENERATOR_INCLUDE_SHIFT_HH

#include <string>
#include <vector>

#include "G4ThreeVector.hh"
#include "ParticleGenerator/include/ShiftMessenger.hh"
#include "Service/include/Logger.hh"

namespace Surface {
/**
 * @brief Shift class moves a position along a set direction.
 * @details The moved distance can be set randomly based on a set probability distribution
 */
class Shift {
 public:
  explicit Shift(VerboseLevel verboseLvl = VerboseLevel::Default);
  explicit Shift(const G4String &filename, VerboseLevel verboseLvl = VerboseLevel::Default);
  ~Shift() = default;
  void DoShift(G4ThreeVector &position, const G4ThreeVector &direction);
  static void DoShiftByValue(G4double shift, G4ThreeVector &position,
                      const G4ThreeVector &direction);
  std::stringstream StreamShiftTable() const;
  void PrintShiftTable();
  void LoadShiftTable(const std::string &filename);
  void SetMinShift(G4double min);
  void SetMaxShift(G4double max);
  void ConfineToMaterial(const G4String &materialName);
  void SetVerboseLvl(VerboseLevel verboseLvl);
  void SetVerboseLvl(G4int verboseLvl);

 private:
  G4double Interpolate(size_t);
  static G4double Interpolate(G4double, G4double, G4double);
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
#endif  // SRC_PARTICLEGENERATOR_INCLUDE_SHIFT_HH
