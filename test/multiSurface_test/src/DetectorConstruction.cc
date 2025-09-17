/**
 * @brief example of combination of modules portal and surface
 * @date 2024-05-21
 * @file DetectorConstruction
 * @author C.Gruener
 */

#include "../../../src/Portal/include/MultipleSubworld.hh"
#include "../../../src/Service/include/G4Voxelizer_Green.hh"
#include "../../../src/Service/include/MultiportalHelper.hh"
#include "DetectorConstruction.hh"
#include <CLHEP/Geometry/Transform3D.h>
#include <G4MultiUnion.hh>
#include <G4UserLimits.hh>
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), fScoringVolume(nullptr) {}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::Construct() {
  // get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  G4bool checkOverlaps = false;

  // setup of the world
  const G4double world_sizeXY = 1 * m;
  const G4double world_sizeZ = 1 * m;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto *solidWorld = new G4Box("World",  // its name
                                0.5 * world_sizeXY, 0.5 * world_sizeXY,
                                0.5 * world_sizeZ);  // its size

  auto *logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                         world_mat,   // its material
                                         "World");    // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(nullptr,                // no rotation
                        G4ThreeVector(),  // at (0,0,0)
                        logicWorld,       // its logical volume
                        "World",          // its name
                        nullptr,                // its mother volume
                        false,            // no boolean operation
                        0,                // copy number
                        checkOverlaps);   // overlaps checking

  // setup of Portal-Subworld setup using the helper class
  Surface::MultiportalHelper helper("MultiSurface");

  // portal size
  helper.SetDxPortal(10 * cm);
  helper.SetDyPortal(10 * cm);
  helper.SetDzPortal(0.5 * cm);

  //subworld size
  helper.SetDxSub(0.5 * mm);
  helper.SetDySub(0.5 * mm);
  helper.SetDzSub(0.5 * cm);

  // number of subworlds in x/y direction
  helper.SetNxSub(200);
  helper.SetNySub(200);

  // placement of subworlds
  const G4ThreeVector placementA{5 * cm, 5 * cm, 22 * cm};
  const G4ThreeVector placementB{5 * cm, 5 * cm, 20 * cm};
  const G4ThreeVector placementC{5 * cm, 5 * cm, 18 * cm};
  const G4Transform3D trafoA{G4RotationMatrix(), placementA};
  const G4Transform3D trafoB{G4RotationMatrix(), placementB};
  const G4Transform3D trafoC{G4RotationMatrix(), placementC};
  helper.AddSubworldPlacement(trafoA);
  helper.AddSubworldPlacement(trafoB);
  helper.AddSubworldPlacement(trafoC);

  // probability of selected subworld when sampled to fill portal representation by subworlds
  helper.AddSubworldDensity(0.3);
  helper.AddSubworldDensity(0.5);
  helper.AddSubworldDensity(0.2);

  // mother volume of all generated volumes
  helper.SetMotherVolume(logicWorld);

  //placement of portal
  const G4ThreeVector placementPortal{0 * cm, 0 * cm, 10 * cm};
  const G4Transform3D trafoPortal{G4RotationMatrix(), placementPortal};
  helper.SetPortalPlacement(trafoPortal);

  // material of subworld
  G4Material *subworldMaterial = nist->FindOrBuildMaterial("G4_AIR");
  helper.SetSubworldMaterial(subworldMaterial);

  // number of subworlds
  helper.SetNDifferentSubworlds(3);

  // portal name
  helper.SetPortalName("MultiSurfacePortal");

  // generate portal-subworld setup
  helper.Generate();

  // build roughness for subworld's
  // size of roughness-basis
  const G4double basis_sizeXY = helper.GetSubworldDx();
  const G4double basis_sizeZ = helper.GetSubworldDz() * 0.3;
  const G4double subworld_sizeZ = helper.GetSubworldDz();
  G4ThreeVector surfaceInSubworld{0, 0, -subworld_sizeZ * 0.5 + basis_sizeZ};
  G4ThreeVector basisPlacement{0, 0, -basis_sizeZ * 0.5};
  G4RotationMatrix basisRotation{G4RotationMatrix()};
  G4Transform3D basisTransform(basisRotation, basisPlacement);

  auto *solidBasis = new G4Box("Basis", 0.5 * basis_sizeXY, 0.5 * basis_sizeXY,
                                0.5 * basis_sizeZ);

  // describer of roughness
  Surface::Describer &describerA = fGeneratorA.GetDescriber();
  Surface::Describer &describerB = fGeneratorB.GetDescriber();
  Surface::Describer &describerC = fGeneratorC.GetDescriber();

  // number of spikes in x/y direction
  describerA.SetNrSpike_X(50);
  describerA.SetNrSpike_Y(50);
  // size of spikes
  describerA.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerA.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerA.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.5);
  // number of spike layers
  describerA.SetNLayer(1);
  // spike height deviation from set mean height
  describerA.SetHeightDeviation(0.0 * mm);
  // spike form
  describerA.SetSpikeform(Surface::Describer::SpikeShape::UniformPyramid);

  describerB.SetNrSpike_X(50);
  describerB.SetNrSpike_Y(50);
  describerB.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerB.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerB.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.3);
  describerB.SetNLayer(1);
  describerB.SetHeightDeviation(0.0 * mm);
  describerB.SetSpikeform(Surface::Describer::SpikeShape::UniformPyramid);

  describerC.SetNrSpike_X(50);
  describerC.SetNrSpike_Y(50);
  describerC.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerC.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerC.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.1);
  describerC.SetNLayer(1);
  describerC.SetHeightDeviation(0.0 * mm);
  describerC.SetSpikeform(Surface::Describer::SpikeShape::UniformPyramid);

  // generate surface
  fGeneratorA.GenerateSurface();
  fGeneratorB.GenerateSurface();
  fGeneratorC.GenerateSurface();

  // convert them to G4Multiunion
  auto *solidSurfaceA =
      dynamic_cast<G4MultiUnion *>(fGeneratorA.GetSolid());
  auto *solidSurfaceB =
      dynamic_cast<G4MultiUnion *>(fGeneratorB.GetSolid());
  auto *solidSurfaceC =
      dynamic_cast<G4MultiUnion *>(fGeneratorC.GetSolid());

  // add roughness-basis to generated surface
  solidSurfaceA->AddNode(*solidBasis, basisTransform);
  solidSurfaceB->AddNode(*solidBasis, basisTransform);
  solidSurfaceC->AddNode(*solidBasis, basisTransform);

  // Voxelize all surfaces (c-style cast necessary as G4Voxelizer is not polymorphic)
  auto &voxelA =
      (Surface::G4Voxelizer_Green &)solidSurfaceA->GetVoxels();
  auto &voxelB =
      (Surface::G4Voxelizer_Green &)solidSurfaceB->GetVoxels();
  auto &voxelC =
      (Surface::G4Voxelizer_Green &)solidSurfaceC->GetVoxels();

  // set boundaries for custom voxelization
  voxelA.SetMaxBoundary(10, 10, 2);
  voxelB.SetMaxBoundary(10, 10, 2);
  voxelC.SetMaxBoundary(10, 10, 2);

  // voxelize
  voxelA.Voxelize(solidSurfaceA);
  voxelB.Voxelize(solidSurfaceB);
  voxelC.Voxelize(solidSurfaceC);

  // get position of subworld
  const G4ThreeVector placementSubA =
      helper.GetSubworldPlacement(0).getTranslation();
  const G4ThreeVector placementSubB =
      helper.GetSubworldPlacement(1).getTranslation();
  const G4ThreeVector placementSubC =
      helper.GetSubworldPlacement(2).getTranslation();

  // calculate the position of facetStore (only 2D surface of roughness)
  const G4ThreeVector facetStorePlacementA = placementSubA + surfaceInSubworld;
  const G4ThreeVector facetStorePlacementB = placementSubB + surfaceInSubworld;
  const G4ThreeVector facetStorePlacementC = placementSubC + surfaceInSubworld;

  // set position of facetStore
  fGeneratorA.SetSurfaceTransformation(facetStorePlacementA);
  fGeneratorB.SetSurfaceTransformation(facetStorePlacementB);
  fGeneratorC.SetSurfaceTransformation(facetStorePlacementC);

  // material of roughness
  G4Material *surface_mat = nist->FindOrBuildMaterial("G4_Si");

  // generate logical volumes of roughness
  auto *logicSurfaceA =
      new G4LogicalVolume(solidSurfaceA, surface_mat, "SurfaceA");
  auto *logicSurfaceB =
      new G4LogicalVolume(solidSurfaceB, surface_mat, "SurfaceB");
  auto *logicSurfaceC =
      new G4LogicalVolume(solidSurfaceC, surface_mat, "SurfaceC");

  //place logical volumes of roughness in subworlds
  Surface::MultipleSubworld *subA = helper.GetSubworld(0);
  Surface::MultipleSubworld *subB = helper.GetSubworld(1);
  Surface::MultipleSubworld *subC = helper.GetSubworld(2);

  new G4PVPlacement(nullptr, surfaceInSubworld, logicSurfaceA, "SurfaceA",
                    subA->GetVolume()->GetLogicalVolume(), false, 0, checkOverlaps);
  new G4PVPlacement(nullptr, surfaceInSubworld, logicSurfaceB, "SurfaceB",
                    subB->GetVolume()->GetLogicalVolume(), false, 0, checkOverlaps);
  new G4PVPlacement(nullptr, surfaceInSubworld, logicSurfaceC, "SurfaceC",
                    subC->GetVolume()->GetLogicalVolume(), false, 0, checkOverlaps);

  // set custom stepLimit
  G4double maxStepsize = 100 * mm;
  G4double maxStepsize_subworld = 100. * mm;
  auto *limit = new G4UserLimits(maxStepsize);
  auto *limit_subworld = new G4UserLimits(maxStepsize_subworld);
  logicWorld->SetUserLimits(limit);
  subA->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  subB->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  subC->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);

  // return the physical World
  return physWorld;
}
