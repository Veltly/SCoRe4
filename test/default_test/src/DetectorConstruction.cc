// Author C.Gruener
// Date 24-05-21
// File DetectorConstruction

#include "DetectorConstruction.hh"

#include <CLHEP/Geometry/Transform3D.h>

#include <G4MultiUnion.hh>
#include <G4Region.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4UserLimits.hh>
#include <cfloat>
#include <limits>

#include "../../../src/Portal/include/MultipleSubworld.hh"
#include "../../../src/Service/include/MultiportalHelper.hh"
#include "../../../src/Service/include/RoughnessHelper.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fPortalHelper("PortalHelper"),
      fRoughnessHelperA("RoughnessHelperA"),
      fRoughnessHelperB("RoughnessHelperB"),
      fRoughnessHelperC("RoughnessHelperC"),
      fScoringVolume(0) {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();
  G4bool checkOverlaps = false;

  //
  // World
  //
  const G4double world_sizeXY = 1 * m;
  const G4double world_sizeZ = 1 * m;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box *solidWorld = new G4Box("World",  // its name
                                0.5 * world_sizeXY, 0.5 * world_sizeXY,
                                0.5 * world_sizeZ);  // its size

  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                                    world_mat,   // its material
                                                    "World");    // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0,                // no rotation
                        G4ThreeVector(),  // at (0,0,0)
                        logicWorld,       // its logical volume
                        "World",          // its name
                        0,                // its mother  volume
                        false,            // no boolean operation
                        0,                // copy number
                        checkOverlaps);   // overlaps checking

  const G4ThreeVector placementA{5 * cm, 5 * cm, 22 * cm};
  const G4ThreeVector placementB{5 * cm, 5 * cm, 20 * cm};
  const G4ThreeVector placementC{5 * cm, 5 * cm, 18 * cm};

  const G4Transform3D trafoA{G4RotationMatrix(), placementA};
  const G4Transform3D trafoB{G4RotationMatrix(), placementB};
  const G4Transform3D trafoC{G4RotationMatrix(), placementC};

  const G4ThreeVector placementPortal{0 * cm, 0 * cm, 10 * cm};
  const G4Transform3D trafoPortal{G4RotationMatrix(), placementPortal};

  fPortalHelper.AddSubworldPlacement(trafoA);
  fPortalHelper.AddSubworldPlacement(trafoB);
  fPortalHelper.AddSubworldPlacement(trafoC);

  fPortalHelper.AddSubworldDensity(0.3);
  fPortalHelper.AddSubworldDensity(0.5);
  fPortalHelper.AddSubworldDensity(0.2);

  fPortalHelper.SetMotherVolume(logicWorld);

  fPortalHelper.SetPortalPlacement(trafoPortal);

  fPortalHelper.SetNDifferentSubworlds(3);

  fPortalHelper.SetPortalName("Portal");

  fRoughnessHelperA.Generate();
  fRoughnessHelperB.Generate();
  fRoughnessHelperC.Generate();

  G4Transform3D trafo{
      G4RotationMatrix(),
      G4ThreeVector(0., 0., -fPortalHelper.GetSubworldDz() + 2. * CLHEP::mm)};

  fPortalHelper.AddRoughness(fRoughnessHelperA.LogicRoughness(), trafo,
                             fRoughnessHelperA.FacetStore());
  fPortalHelper.AddRoughness(fRoughnessHelperB.LogicRoughness(), trafo,
                             fRoughnessHelperB.FacetStore());
  fPortalHelper.AddRoughness(fRoughnessHelperC.LogicRoughness(), trafo,
                             fRoughnessHelperC.FacetStore());

  fPortalHelper.Generate();

  CheckValues();

  Surface::MultipleSubworld *subA = fPortalHelper.GetSubworld(0);
  Surface::MultipleSubworld *subB = fPortalHelper.GetSubworld(1);
  Surface::MultipleSubworld *subC = fPortalHelper.GetSubworld(2);
  //  set StepLimit

  G4double maxStepsize = 1 * CLHEP::mm;
  G4double maxStepsize_subworld = 100. * CLHEP::nm;
  G4UserLimits *limit = new G4UserLimits(maxStepsize);
  G4UserLimits *limit_subworld = new G4UserLimits(maxStepsize_subworld);
  logicWorld->SetUserLimits(limit);
  subA->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  subB->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  subC->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  //
  //   always return the physical World
  //
  return physWorld;
}

void DetectorConstruction::CheckValues() {
  auto isSame = [](const G4double valA, const G4double valB) {
    const G4double numericLimit = std::numeric_limits<G4double>::epsilon() * 10;
    return std::fabs(valA - valB) < numericLimit;
  };

  const G4double subDx{fPortalHelper.GetSubworldDx()};
  const G4double subDy{fPortalHelper.GetSubworldDy()};
  const G4double subDz{fPortalHelper.GetSubworldDz()};

  const G4double spikeDxA{fRoughnessHelperA.GetSpikeDx()};
  const G4double spikeDyA{fRoughnessHelperA.GetSpikeDy()};
  const G4double spikeHeightA{fRoughnessHelperA.GetSpikeMeanHeight()};
  const G4int spikeNxA{fRoughnessHelperA.GetSpikeNx()};
  const G4int spikeNyA{fRoughnessHelperA.GetSpikeNy()};
  const G4double basisDzA{fRoughnessHelperA.GetBasisHeight()};

  const G4double spikeDxB{fRoughnessHelperB.GetSpikeDx()};
  const G4double spikeDyB{fRoughnessHelperB.GetSpikeDy()};
  const G4double spikeHeightB{fRoughnessHelperB.GetSpikeMeanHeight()};
  const G4int spikeNxB{fRoughnessHelperB.GetSpikeNx()};
  const G4int spikeNyB{fRoughnessHelperB.GetSpikeNy()};
  const G4double basisDzB{fRoughnessHelperC.GetBasisHeight()};

  const G4double spikeDxC{fRoughnessHelperC.GetSpikeDx()};
  const G4double spikeDyC{fRoughnessHelperC.GetSpikeDy()};
  const G4double spikeHeightC{fRoughnessHelperC.GetSpikeMeanHeight()};
  const G4int spikeNxC{fRoughnessHelperC.GetSpikeNx()};
  const G4int spikeNyC{fRoughnessHelperC.GetSpikeNy()};
  const G4double basisDzC{fRoughnessHelperC.GetBasisHeight()};

  G4bool error{false};
  std::stringstream ss;
  ss << "\n";

  //
  // Roughness A
  //

  if (!isSame(spikeDxA * spikeNxA, subDx)) {
    ss << "RoughnessA:\n";
    ss << "Spikes do not fit in Subworld (X-Dimension)!\n";
    ss << "SpikeDx * SpikesNx != SubworldDx\n";
    ss << spikeDxA / CLHEP::mm << " mm * " << spikeNxA
       << " != " << subDx / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (!isSame(spikeDyA * spikeNyA, subDy)) {
    ss << "RoughnessA:\n";
    ss << "Spikes do not fit in Subworld (Y-Dimension)!\n";
    ss << "SpikeDy * SpikesNy != SubworldDy\n";
    ss << spikeDyA / CLHEP::mm << " mm * " << spikeNyA
       << " != " << subDy / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (!isSame(spikeHeightA + basisDzA, subDz)) {
    ss << "RoughnessA:\n";
    ss << "Spikes do not fit in Subworld (Z-Dimension)!\n";
    ss << "SpikeHeight + BasisHeight != SubworldDz\n";
    ss << spikeHeightA / CLHEP::mm << " mm + " << basisDzA / CLHEP::mm
       << " mm != " << subDz / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  //
  // Roughness B
  //

  if (!isSame(spikeDxB * spikeNxB, subDx)) {
    ss << "RoughnessB:\n";
    ss << "Spikes do not fit in Subworld (X-Dimension)!\n";
    ss << "SpikeDx * SpikesNx != SubworldDx\n";
    ss << spikeDxB / CLHEP::mm << " mm * " << spikeNxB
       << " != " << subDx / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (!isSame(spikeDyB * spikeNyB, subDy)) {
    ss << "RoughnessB:\n";
    ss << "Spikes do not fit in Subworld (Y-Dimension)!\n";
    ss << "SpikeDy * SpikesNy != SubworldDy\n";
    ss << spikeDyB / CLHEP::mm << " mm * " << spikeNyB
       << " != " << subDy / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (!isSame(spikeHeightB + basisDzB, subDz)) {
    ss << "RoughnessB:\n";
    ss << "Spikes do not fit in Subworld (Z-Dimension)!\n";
    ss << "SpikeHeight + BasisHeight != SubworldDz\n";
    ss << spikeHeightB / CLHEP::mm << " mm + " << basisDzB / CLHEP::mm
       << " mm != " << subDz / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  //
  // Roughness C
  //

  if (!isSame(spikeDxC * spikeNxC, subDx)) {
    ss << "RoughnessC:\n";
    ss << "Spikes do not fit in Subworld (X-Dimension)!\n";
    ss << "SpikeDx * SpikesNx != SubworldDx\n";
    ss << spikeDxC / CLHEP::mm << " mm * " << spikeNxC
       << " != " << subDx / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (!isSame(spikeDyC * spikeNyC, subDy)) {
    ss << "RoughnessC:\n";
    ss << "Spikes do not fit in Subworld (Y-Dimension)!\n";
    ss << "SpikeDy * SpikesNy != SubworldDy\n";
    ss << spikeDyC / CLHEP::mm << " mm * " << spikeNyC
       << " != " << subDy / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (!isSame(spikeHeightC + basisDzC, subDz)) {
    ss << "RoughnessC:\n";
    ss << "Spikes do not fit in Subworld (Z-Dimension)!\n";
    ss << "SpikeHeight + BasisHeight != SubworldDz\n";
    ss << spikeHeightC / CLHEP::mm << " mm + " << basisDzC / CLHEP::mm
       << " mm != " << subDz / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (error) {
    std::stringstream stream;
    stream << "\n";
    stream << "**************************************************\n";
    stream << "*                                                *\n";
    stream << "*        Error while generating Detector!        *\n";
    stream << "*                                                *\n";
    stream << "**************************************************\n";

    ss << "\n";
    ss << "**************************************************\n";
    ss << "**************************************************\n";
    G4cout << stream.str() << ss.str() << G4endl;
    exit(EXIT_FAILURE);
  }
}
