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

class RoughnessHelperMessenger;

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
  inline const G4String &GetName() const { return fName; }
  inline G4double GetVerboseLvl() { return fLogger.GetVerboseLvl(); }
  inline G4double GetSpikeDx() { return fDxSpike; }
  inline G4double GetSpikeDy() { return fDySpike; }
  inline G4double GetSpikeMeanHeight() { return fDzSpikeMean; }
  inline G4double GetSpikeDevHeight() { return fDzSpikeDev; }
  inline G4int GetSpikeNx() { return fNxSpike; }
  inline G4int GetSpikeNy() { return fNySpike; }
  inline auto GetSpikeform() { return fSpikeform; }
  inline G4double GetBasisDx() { return fDxBasis; }
  inline G4double GetBasisDy() { return fDyBasis; }
  inline G4double GetBasisHeight() { return fDzBasis; }
  inline const G4Material *GetMaterial() { return fMaterial; }
  inline G4int GetBoundaryX() { return fNxBoundary; }
  inline G4int GetBoundaryY() { return fNyBoundary; }
  inline G4int GetBoundaryZ() { return fNzBoundary; }
  inline auto GetStepLimit() { return fStepLimit; }

  // Setter
  void SetVerbose(const G4int verboseLvl);
  void SetSpikeDx(const G4double);
  void SetSpikeDy(const G4double);
  void SetSpikeMeanHeight(const G4double);
  void SetSpikeHeightDeviation(const G4double);
  void SetSpikeform(const Describer::Spikeform);
  void SetSpikeform(const G4String &);

  void SetSpikeNx(const G4int);
  void SetSpikeNy(const G4int);
  void SetSpikeNLayer(const G4int);

  void SetBasisDx(const G4double);
  void SetBasisDy(const G4double);
  void SetBasisHeight(const G4double);

  void SetMaterial(G4Material *);
  void SetMaterial(const G4String &);

  void SetBoundaryX(const G4int val);
  void SetBoundaryY(const G4int val);
  void SetBoundaryZ(const G4int val);

  void SetStepLimit(const G4double val);

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
  RoughnessHelperMessenger *fMessenger;

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
