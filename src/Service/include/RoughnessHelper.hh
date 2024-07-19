// Copyright [2024] C.Gruener
// Date: 24-07-15
// File: Helper class to build a roughness G4LogicVolume object

#ifndef SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH_
#define SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH_

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MultiUnion.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/Generator.hh"

namespace Surface {

typedef Describer::Spikeform Spikeform;

class RoughnessHelper {
 public:
  RoughnessHelper(const G4String &name);
  RoughnessHelper(const G4String &name, const G4int verboseLvl);

  void Generate();
  // Getter
  Describer &Describer();
  G4MultiUnion *SolidRoughness() const;
  G4LogicalVolume *LogicRoughness() const;
  FacetStore *FacetStore();
  // Setter
  void SpikeWidth_X(const G4double);
  void SpikeWidth_Y(const G4double);
  void SpikeMeanHeight(const G4double);
  void SpikeHeightDeviation(const G4double);
  void Spikeform(const Describer::Spikeform);

  void NSpike_X(const G4int);
  void NSpike_Y(const G4int);
  void NLayer(const G4int);

  void BasisWidth_X(const G4double);
  void BasisWidth_Y(const G4double);
  void BasisHeight(const G4double);

  void Material(G4Material *);

  void BoundaryX(const G4int val);
  void BoundaryY(const G4int val);
  void BoundaryZ(const G4int val);

  void StepLimit(const G4double val);

 private:
  void CheckValues();
  void BuildSurface();
  void BuildBasis();
  void Finalize();

 private:
  // Ctrl
  Logger fLogger;
  SurfaceGenerator fGenerator;
  G4MultiUnion *fRoughness{nullptr};
  G4LogicalVolume *fLogicRoughness{nullptr};

  // General
  const G4String fName;
  G4UserLimits *fStepLimit{nullptr};

  // Spike
  G4double fDxSpike{0};
  G4double fDySpike{0};
  G4double fDzSpikeMean{0};
  G4double fDzSpikeDev{0};
  G4int fNxSpike{0};
  G4int fNySpike{0};
  G4int fNLayer{1};
  Describer::Spikeform fSpikeform;

  // Bulk
  G4double fDxBasis{0};
  G4double fDyBasis{0};
  G4double fDzBasis{0};

  G4Material *fMaterial{nullptr};

  // Voxelizer
  G4int fNxBoundary{100000};
  G4int fNyBoundary{100000};
  G4int fNzBoundary{100000};
};

}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH_
