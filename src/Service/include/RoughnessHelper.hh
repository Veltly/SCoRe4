// Copyright [2024] C.Gruener
// Date: 24-07-15
// File: Helper class to build a roughness G4LogicVolume object

#ifndef SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH_
#define SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH_

#include <G4MultiUnion.hh>

#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/Generator.hh"

namespace Surface {

class RoughnessHelper {
 public:
  RoughnessHelper(const G4int verboseLvl = 6);
  void Generate();
  // Getter
  Describer &Describer();
  G4MultiUnion *Roughness() const;

 private:
  void CheckValues();
  void BuildBasis();
  void BuildSurface();
  void Finalize();

 private:
  Logger fLogger;
  SurfaceGenerator fGenerator;
  G4MultiUnion *fRoughness;
};

}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH_
