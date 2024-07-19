// Copyright [2024] C.Gruener
// Date: 24-07-15
// File: Helper class to build a roughness G4LogicVolume object

#include "Service/include/RoughnessHelper.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiUnion.hh"
#include "G4UserLimits.hh"
#include "Service/include/G4Voxelizer_Green.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/Generator.hh"

Surface::RoughnessHelper::RoughnessHelper(const G4String &name)
    : fLogger("RoughnessHelper_" + name),
      fGenerator(name),
      fRoughness(nullptr),
      fLogicRoughness(nullptr),
      fName(name),
      fStepLimit(nullptr),
      fMaterial(nullptr) {}

Surface::RoughnessHelper::RoughnessHelper(const G4String &name,
                                          const G4int verboseLvl)
    : fLogger("RoughnessHelper_" + name, verboseLvl),
      fGenerator(name, verboseLvl),
      fRoughness(nullptr),
      fLogicRoughness(nullptr),
      fName(name),
      fStepLimit(nullptr),
      fMaterial(nullptr) {}

void Surface::RoughnessHelper::Generate() {
  CheckValues();

  BuildSurface();

  BuildBasis();

  Finalize();

  fLogger.WriteInfo("Build Roughness " + fName);
}

// Getter
Surface::Describer &Surface::RoughnessHelper::Describer() {
  return fGenerator.GetDescriber();
}

G4MultiUnion *Surface::RoughnessHelper::SolidRoughness() const {
  return fRoughness;
}

G4LogicalVolume *Surface::RoughnessHelper::LogicRoughness() const {
  return new G4LogicalVolume(fRoughness, fMaterial, fName);
}

void Surface::RoughnessHelper::SpikeWidth_X(const G4double val) {
  fDxSpike = val;
}

void Surface::RoughnessHelper::SpikeWidth_Y(const G4double val) {
  fDySpike = val;
}

void Surface::RoughnessHelper::SpikeMeanHeight(const G4double val) {
  fDzSpikeMean = val;
}

void Surface::RoughnessHelper::SpikeHeightDeviation(const G4double val) {
  fDzSpikeDev = val;
}

void Surface::RoughnessHelper::Spikeform(
    const Surface::Describer::Spikeform form) {
  fSpikeform = form;
}

void Surface::RoughnessHelper::NSpike_X(const G4int val) { fNxSpike = val; }

void Surface::RoughnessHelper::NSpike_Y(const G4int val) { fNySpike = val; }

void Surface::RoughnessHelper::NLayer(const G4int val) { fNLayer = val; }

void Surface::RoughnessHelper::BasisWidth_X(const G4double val) {
  fDxBasis = val;
}

void Surface::RoughnessHelper::BasisWidth_Y(const G4double val) {
  fDyBasis = val;
}

void Surface::RoughnessHelper::BasisHeight(const G4double val) {
  fDzBasis = val;
}

void Surface::RoughnessHelper::Material(G4Material *mat) { fMaterial = mat; }

void Surface::RoughnessHelper::BoundaryX(const G4int val) { fNxBoundary = val; }
void Surface::RoughnessHelper::BoundaryY(const G4int val) { fNyBoundary = val; }
void Surface::RoughnessHelper::BoundaryZ(const G4int val) { fNzBoundary = val; }

void Surface::RoughnessHelper::StepLimit(const G4double val) {
  fStepLimit = new G4UserLimits(val);
}

void Surface::RoughnessHelper::CheckValues() {
  // TODO: Implement checks for values
  if (fStepLimit == nullptr) {
    const G4double min = std::min(fDxSpike, fDySpike);
    fStepLimit = new G4UserLimits(0.01 * min);
    fLogger.WriteDetailInfo("Minimum set to " + std::to_string(0.01 * min));
  }
}

void Surface::RoughnessHelper::BuildSurface() {
  // Prepare describer
  Surface::Describer &describer = fGenerator.GetDescriber();
  describer.SetNrSpike_X(fNxSpike);
  describer.SetNrSpike_Y(fNySpike);
  describer.SetSpikeWidth_X(fDxSpike * 0.5);
  describer.SetSpikeWidth_Y(fDySpike * 0.5);
  describer.SetMeanHeight(fDzSpikeMean * 0.5);
  describer.SetNLayer(fNLayer);
  describer.SetHeightDeviation(fDzSpikeDev);
  describer.SetSpikeform(fSpikeform);

  fGenerator.GenerateSurface();

  fRoughness = static_cast<G4MultiUnion *>(fGenerator.GetSolid());
  fLogger.WriteDetailInfo("Generated roughness");
}

void Surface::RoughnessHelper::BuildBasis() {
  // Generate Basis
  const G4String nameBox = fName + "_Box";
  G4Box *solidBasis =
      new G4Box(nameBox, fDxBasis * 0.5, fDyBasis * 0.5, fDzBasis * 0.5);
  // Generate Trafo
  const G4ThreeVector placement{0., 0., -fDzBasis};
  G4Transform3D trafo{G4RotationMatrix(), placement};
  // Add to Roughness
  fRoughness->AddNode(*solidBasis, trafo);

  fLogger.WriteDetailInfo("Added basis to roughness");
}

void Surface::RoughnessHelper::Finalize() {
  Surface::G4Voxelizer_Green &voxelizer =
      (Surface::G4Voxelizer_Green &)fRoughness->GetVoxels();
  voxelizer.SetMaxBoundary(fNxBoundary, fNyBoundary, fNzBoundary);
  voxelizer.Voxelize(fRoughness);
  std::string name = fName + "_roughness";
  fLogicRoughness = new G4LogicalVolume(fRoughness, fMaterial, name);
  fLogicRoughness->SetUserLimits(fStepLimit);
  fLogger.WriteDetailInfo("Build Logical Volume " + name);
}

Surface::FacetStore *Surface::RoughnessHelper::FacetStore() {
  return fGenerator.GetFacetStore();
}
