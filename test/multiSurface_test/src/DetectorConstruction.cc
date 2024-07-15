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

#include "../../../src/Portal/include/MultipleSubworld.hh"
#include "../../../src/Service/include/G4Voxelizer_Green.hh"
#include "../../../src/Service/include/MultiportalHelper.hh"
#include "../../../src/SurfaceGenerator/include/Describer.hh"
#include "../../../src/SurfaceGenerator/include/Generator.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), fScoringVolume(0) {}

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

  G4Material *subworldMaterial = nist->FindOrBuildMaterial("G4_AIR");

  Surface::MultiportalHelper helper(6);

  helper.SetDxPortal(10 * cm);
  helper.SetDyPortal(10 * cm);
  helper.SetDzPortal(0.5 * cm);

  helper.SetDxSub(0.5 * mm);
  helper.SetDySub(0.5 * mm);
  helper.SetDzSub(0.5 * cm);

  helper.SetNxSub(200);
  helper.SetNySub(200);

  helper.AddSubworldPlacement(trafoA);
  helper.AddSubworldPlacement(trafoB);
  helper.AddSubworldPlacement(trafoC);

  helper.AddSubworldDensity(0.3);
  helper.AddSubworldDensity(0.5);
  helper.AddSubworldDensity(0.2);

  helper.SetMotherVolume(logicWorld);

  helper.SetPortalPlacement(trafoPortal);

  helper.SetSubworldMaterial(subworldMaterial);

  helper.SetNDifferentSubworlds(3);

  helper.SetPortalName("Test");
  helper.Generate();

  G4cout << "TestC" << G4endl;
  // build roughness for subworlds
  const G4double basis_sizeXY = helper.GetSubworldDx();
  const G4double basis_sizeZ = helper.GetSubworldDz() * 0.3;
  const G4double subworld_sizeZ = helper.GetSubworldDz();

  G4Material *surface_mat = nist->FindOrBuildMaterial("G4_Si");

  G4ThreeVector surfaceInSubworld{0, 0, -subworld_sizeZ * 0.5 + basis_sizeZ};
  G4ThreeVector basisPlacement{0, 0, -basis_sizeZ * 0.5};
  G4RotationMatrix basisRotation{G4RotationMatrix()};
  G4Transform3D basisTransform(basisRotation, basisPlacement);

  G4Box *solidBasis = new G4Box("Basis", 0.5 * basis_sizeXY, 0.5 * basis_sizeXY,
                                0.5 * basis_sizeZ);

  Surface::Describer &describerA = fGeneratorA.GetDescriber();
  Surface::Describer &describerB = fGeneratorB.GetDescriber();
  Surface::Describer &describerC = fGeneratorC.GetDescriber();

  describerA.SetNrSpike_X(50);
  describerA.SetNrSpike_Y(50);
  describerA.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerA.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerA.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.5);
  describerA.SetNLayer(1);
  describerA.SetHeightDeviation(0.0 * mm);
  describerA.SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);

  describerB.SetNrSpike_X(50);
  describerB.SetNrSpike_Y(50);
  describerB.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerB.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerB.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.3);
  describerB.SetNLayer(1);
  describerB.SetHeightDeviation(0.0 * mm);
  describerB.SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);

  describerC.SetNrSpike_X(50);
  describerC.SetNrSpike_Y(50);
  describerC.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerC.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerC.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.1);
  describerC.SetNLayer(1);
  describerC.SetHeightDeviation(0.0 * mm);
  describerC.SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);

  //
  // description of all surfaces finished, now generate them
  fGeneratorA.GenerateSurface();
  fGeneratorB.GenerateSurface();
  fGeneratorC.GenerateSurface();

  G4MultiUnion *solidSurfaceA =
      static_cast<G4MultiUnion *>(fGeneratorA.GetSolid());
  G4MultiUnion *solidSurfaceB =
      static_cast<G4MultiUnion *>(fGeneratorB.GetSolid());
  G4MultiUnion *solidSurfaceC =
      static_cast<G4MultiUnion *>(fGeneratorC.GetSolid());

  solidSurfaceA->AddNode(*solidBasis, basisTransform);
  solidSurfaceB->AddNode(*solidBasis, basisTransform);
  solidSurfaceC->AddNode(*solidBasis, basisTransform);

  // Voxelize all surfaces
  //

  Surface::G4Voxelizer_Green &voxelA =
      (Surface::G4Voxelizer_Green &)solidSurfaceA->GetVoxels();
  Surface::G4Voxelizer_Green &voxelB =
      (Surface::G4Voxelizer_Green &)solidSurfaceB->GetVoxels();
  Surface::G4Voxelizer_Green &voxelC =
      (Surface::G4Voxelizer_Green &)solidSurfaceC->GetVoxels();

  voxelA.SetMaxBoundary(10, 10, 2);
  voxelB.SetMaxBoundary(10, 10, 2);
  voxelC.SetMaxBoundary(10, 10, 2);

  voxelA.Voxelize(solidSurfaceA);
  voxelB.Voxelize(solidSurfaceB);
  voxelC.Voxelize(solidSurfaceC);

  const G4ThreeVector placementSubA =
      helper.GetSubworldPlacement(0).getTranslation();
  const G4ThreeVector placementSubB =
      helper.GetSubworldPlacement(1).getTranslation();
  const G4ThreeVector placementSubC =
      helper.GetSubworldPlacement(2).getTranslation();

  const G4ThreeVector facetStorePlacementA = placementSubA + surfaceInSubworld;
  const G4ThreeVector facetStorePlacementB = placementSubB + surfaceInSubworld;
  const G4ThreeVector facetStorePlacementC = placementSubC + surfaceInSubworld;

  fGeneratorA.SetSurfaceTransformation(facetStorePlacementA);
  fGeneratorB.SetSurfaceTransformation(facetStorePlacementB);
  fGeneratorC.SetSurfaceTransformation(facetStorePlacementC);
  // generate logical volumes of roughness
  //
  G4LogicalVolume *logicSurfaceA =
      new G4LogicalVolume(solidSurfaceA, surface_mat, "SurfaceA");
  G4LogicalVolume *logicSurfaceB =
      new G4LogicalVolume(solidSurfaceB, surface_mat, "SurfaceB");
  G4LogicalVolume *logicSurfaceC =
      new G4LogicalVolume(solidSurfaceC, surface_mat, "SurfaceC");

  Surface::MultipleSubworld *subA = helper.GetSubworld(0);
  Surface::MultipleSubworld *subB = helper.GetSubworld(1);
  Surface::MultipleSubworld *subC = helper.GetSubworld(2);

  new G4PVPlacement(NULL, surfaceInSubworld, logicSurfaceA, "SurfaceA",
                    subA->GetVolume()->GetLogicalVolume(), 0, 0, checkOverlaps);
  new G4PVPlacement(NULL, surfaceInSubworld, logicSurfaceB, "SurfaceB",
                    subB->GetVolume()->GetLogicalVolume(), 0, 0, checkOverlaps);
  new G4PVPlacement(NULL, surfaceInSubworld, logicSurfaceC, "SurfaceC",
                    subC->GetVolume()->GetLogicalVolume(), 0, 0, checkOverlaps);
  //   set StepLimit

  G4double maxStepsize = 100 * mm;
  G4double maxStepsize_subworld = 100. * mm;
  G4UserLimits *limit = new G4UserLimits(maxStepsize);
  G4UserLimits *limit_subworld = new G4UserLimits(maxStepsize_subworld);
  logicWorld->SetUserLimits(limit);
  // subA->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  // subB->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  // subC->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  //
  //    always return the physical World
  //
  return physWorld;
}
