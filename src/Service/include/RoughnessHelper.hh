/**
 * @brief Helper class to build a roughness object
 * @author C.Gruener
 * @date 2024-07-15
 * @file RoughnessHelper.hh
 */

#ifndef SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH
#define SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MultiUnion.hh"
#include "Service/include/Logger.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/Generator.hh"

namespace Surface {

class RoughnessHelperMessenger;

typedef Describer::SpikeShape Spikeform;
/**
 * @brief RoughnessHelper class helps the user to build a roughness object
 * @details The class controls the instantiation of all needed classes, passes
 * the set description, starts the generation of the roughness object and handles
 * the voxelization with the adapted voxelizer class
 */
class RoughnessHelper {
 public:
  explicit RoughnessHelper(const G4String &name);
  RoughnessHelper(const G4String &name, G4int verboseLvl);

  void Generate();
  // Getter
  Describer &Describer();
  G4MultiUnion *SolidRoughness() const;
  G4LogicalVolume *LogicRoughness() const;
  FacetStore *FacetStore();
  inline const G4String &GetName() const { return fName; }
  inline G4double GetVerboseLvl() const { return fLogger.GetVerboseLvl(); }
  inline G4double GetSpikeDx() const { return fDxSpike; }
  inline G4double GetSpikeDy() const { return fDySpike; }
  inline G4double GetSpikeMeanHeight() const { return fDzSpikeMean; }
  inline G4double GetSpikeDevHeight() const { return fDzSpikeDev; }
  inline G4int GetSpikeNx() const { return fNxSpike; }
  inline G4int GetSpikeNy() const { return fNySpike; }
  inline auto GetSpikeform() const { return fSpikeform; }
  inline G4double GetBasisDx() const { return fDxBasis; }
  inline G4double GetBasisDy() const { return fDyBasis; }
  inline G4double GetBasisHeight() const { return fDzBasis; }
  inline const G4Material *GetMaterial() const { return fMaterial; }
  inline G4int GetBoundaryX() const { return fNxBoundary; }
  inline G4int GetBoundaryY() const { return fNyBoundary; }
  inline G4int GetBoundaryZ() const { return fNzBoundary; }
  inline auto GetStepLimit() const { return fStepLimit; }

  // Setter
  void SetVerbose(G4int verboseLvl);
  void SetSpikeDx(G4double);
  void SetSpikeDy(G4double);
  void SetSpikeMeanHeight(G4double);
  void SetSpikeHeightDeviation(G4double);
  void SetSpikeform(Describer::SpikeShape);
  void SetSpikeform(const G4String &);

  void SetSpikeNx(G4int);
  void SetSpikeNy(G4int);
  void SetSpikeNLayer(G4int);

  void SetBasisDx(G4double);
  void SetBasisDy(G4double);
  void SetBasisHeight(G4double);

  void SetMaterial(G4Material *);
  void SetMaterial(const G4String &);

  void SetBoundaryX(G4int val);
  void SetBoundaryY(G4int val);
  void SetBoundaryZ(G4int val);

  void SetStepLimit(G4double val);

 private:
  void CheckValues();
  void BuildSurface();
  void BuildBasis();
  void Finalize();

 private:
  // Control
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
  Describer::SpikeShape fSpikeform{Spikeform::StandardPyramid};

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
#endif  // SRC_SERVICE_INCLUDE_ROUGHNESSHELPER_HH
