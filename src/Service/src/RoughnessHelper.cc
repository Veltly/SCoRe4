// Copyright [2024] C.Gruener
// Date: 24-07-15
// File: Helper class to build a roughness G4LogicVolume object

#include "Service/include/RoughnessHelper.hh"

#include <cstdlib>

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiUnion.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "Service/include/G4Voxelizer_Green.hh"
#include "Service/include/RoughnessHelperMessenger.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/Generator.hh"

Surface::RoughnessHelper::RoughnessHelper(const G4String &name)
    : fLogger("RoughnessHelper_" + name),
      fGenerator(name),
      fRoughness(nullptr),
      fLogicRoughness(nullptr),
      fMessenger(new RoughnessHelperMessenger(this, name)),
      fName(name),
      fStepLimit(nullptr),
      fMaterial(nullptr) {}

Surface::RoughnessHelper::RoughnessHelper(const G4String &name,
                                          const G4int verboseLvl)
    : fLogger("RoughnessHelper_" + name, verboseLvl),
      fGenerator(name, verboseLvl),
      fRoughness(nullptr),
      fLogicRoughness(nullptr),
      fMessenger(new RoughnessHelperMessenger(this, name)),
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
  if (fLogicRoughness == nullptr) {
    fLogger.WriteError("Logic object not build!");
    exit(EXIT_FAILURE);
  }
  return fLogicRoughness;
}

void Surface::RoughnessHelper::SetVerbose(const G4int verboseLvl) {
  fLogger.SetVerboseLvl(verboseLvl);
}

void Surface::RoughnessHelper::SetSpikeDx(const G4double val) {
  fDxSpike = val;
}

void Surface::RoughnessHelper::SetSpikeDy(const G4double val) {
  fDySpike = val;
}

void Surface::RoughnessHelper::SetSpikeMeanHeight(const G4double val) {
  fDzSpikeMean = val;
}

void Surface::RoughnessHelper::SetSpikeHeightDeviation(const G4double val) {
  fDzSpikeDev = val;
}

void Surface::RoughnessHelper::SetSpikeform(
    const Surface::Describer::Spikeform form) {
  fSpikeform = form;
}

void Surface::RoughnessHelper::SetSpikeform(const G4String &spikeform) {
  if (spikeform == "Bump") {
    SetSpikeform(Surface::Describer::Spikeform::Bump);
    return;
  } else if (spikeform == "Peak") {
    SetSpikeform(Surface::Describer::Spikeform::Peak);
    return;
  } else if (spikeform == "UniformPyramide") {
    SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);
    return;
  } else if (spikeform == "StandardPyramide") {
    SetSpikeform(Surface::Describer::Spikeform::StandardPyramide);
    return;
  }

  std::stringstream ss;
  ss << "\n";
  ss << "ERROR\n";
  ss << "Selected spikeform is not valid: " << spikeform << "\n";
  ss << "Valid forms are:\n";
  ss << "\n";
  ss << "Bump\n";
  ss << "Peak\n";
  ss << "UniformPyramide\n";
  ss << "StandardPyramide\n";
  ss << "\n";
  fLogger.WriteError(ss.str());
  exit(EXIT_FAILURE);
}

void Surface::RoughnessHelper::SetSpikeNx(const G4int val) { fNxSpike = val; }

void Surface::RoughnessHelper::SetSpikeNy(const G4int val) { fNySpike = val; }

void Surface::RoughnessHelper::SetSpikeNLayer(const G4int val) {
  fNLayer = val;
}

void Surface::RoughnessHelper::SetBasisDx(const G4double val) {
  fDxBasis = val;
}

void Surface::RoughnessHelper::SetBasisDy(const G4double val) {
  fDyBasis = val;
}

void Surface::RoughnessHelper::SetBasisHeight(const G4double val) {
  fDzBasis = val;
}

void Surface::RoughnessHelper::SetMaterial(G4Material *material) {
  fMaterial = material;
}
void Surface::RoughnessHelper::SetMaterial(const G4String &materialName) {
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *material = nist->FindOrBuildMaterial(materialName);
  SetMaterial(material);
}

void Surface::RoughnessHelper::SetBoundaryX(const G4int val) {
  fNxBoundary = val;
}
void Surface::RoughnessHelper::SetBoundaryY(const G4int val) {
  fNyBoundary = val;
}
void Surface::RoughnessHelper::SetBoundaryZ(const G4int val) {
  fNzBoundary = val;
}

void Surface::RoughnessHelper::SetStepLimit(const G4double val) {
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
  describer.SetSpikeWidth_X(fDxSpike);
  describer.SetSpikeWidth_Y(fDySpike);
  describer.SetMeanHeight(fDzSpikeMean);
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
  G4Box *solidBasis = new G4Box(nameBox, fDxBasis, fDyBasis, fDzBasis);
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
