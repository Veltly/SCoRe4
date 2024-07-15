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
  RoughnessHelper(const G4int verboseLvl = 6);
  void Generate();
  // Getter
  Describer &Describer();
  G4MultiUnion *Roughness() const;

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
  G4double fBasis_Z;
  SurfaceGenerator fGenerator;
  G4MultiUnion *fRoughness;
  G4LogicalVolume *fLogicRoughness;

  // General
  G4String fName;
  G4UserLimits *fStepLimit;

  // Spike
  G4double fDxSpike;
  G4double fDySpike;
  G4double fDzSpikeMean;
  G4double fDzSpikeDev;
  G4int fNxSpike;
  G4int fNySpike;
  G4int fNLayer;
  Describer::Spikeform fSpikeform;

  // Bulk
  G4double fDxBasis;
  G4double fDyBasis;
  G4double fDzBasis;

  G4Material *fMaterial;

  // Voxelizer
  G4int fNxBoundary;
  G4int fNyBoundary;
  G4int fNzBoundary;
};

}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH_
