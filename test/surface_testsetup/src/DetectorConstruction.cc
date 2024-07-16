// Copyright [2024] C.Gruener
// Date 24-05-21
// File DetectorConstruction

#include "../../surface_testsetup/include/DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiUnion.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "ParticleGenerator/include/SurfaceSource.hh"
#include "Service/include/G4Voxelizer_Green.hh"
#include "Service/include/Locator.hh"
#include "SurfaceGenerator/include/Describer.hh"
#include "SurfaceGenerator/include/Generator.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fScoringVolume(0),
      fSurfaceGenerator(Surface::SurfaceGenerator("Surface_Testsetup")) {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  G4bool checkOverlaps = false;

  //
  // World
  //
  G4double world_sizeXY = 1 * m;
  G4double world_sizeZ = 1 * m;
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

  //
  // Subworld to place patch of Rough surface
  //
  //
  G4double subworld_sizeXY = 1 * cm;
  G4double subworld_sizeZ = 1 * cm;
  // G4ThreeVector subworldPlacement{40 * cm, 40 * cm, 40 * cm};
  G4ThreeVector subworldPlacement{0 * cm, 0 * cm, 0 * cm};
  G4Material *subworld_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box *solidSubworld = new G4Box("Subworld",  // its name
                                   0.5 * subworld_sizeXY, 0.5 * subworld_sizeXY,
                                   0.5 * subworld_sizeZ);  // its size

  G4LogicalVolume *logicSubworld =
      new G4LogicalVolume(solidSubworld,  // its solid
                          subworld_mat,   // its material
                          "Subworld");    // its name

  new G4PVPlacement(0,                  // no rotation
                    subworldPlacement,  //
                    logicSubworld,      // its logical volume
                    "Subworld",         // its name
                    logicWorld,         // its mother  volume
                    false,              // no boolean operation
                    0,                  // copy number
                    checkOverlaps);     // overlaps checking

  ///////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////
  //
  //
  // Patch of rough surface
  //
  //

  G4Material *roughness_mat = nist->FindOrBuildMaterial("G4_Si");
  //
  // Basis
  //
  //

  G4double basis_sizeXY = 2 * mm;
  G4double basis_sizeZ = 1 * mm;
  G4Box *solidBasis = new G4Box("Basis", 0.5 * basis_sizeXY, 0.5 * basis_sizeXY,
                                0.5 * basis_sizeZ);

  G4ThreeVector basisPlacement{0., 0., -basis_sizeZ * 0.5};
  G4RotationMatrix basisRotation{G4RotationMatrix()};
  G4Transform3D basisTransform(basisRotation, basisPlacement);
  //
  // Roughness
  //
  //
  // Set options for roughness
  Surface::Describer &describer = fSurfaceGenerator.GetDescriber();
  describer.SetNrSpike_X(2);
  describer.SetNrSpike_Y(2);
  describer.SetSpikeWidth_X(0.5 * mm);
  describer.SetSpikeWidth_Y(0.5 * mm);
  describer.SetMeanHeight(0.5 * mm);
  describer.SetNLayer(1);
  describer.SetHeightDeviation(0.1 * mm);
  describer.SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);

  fSurfaceGenerator.GenerateSurface();
  G4MultiUnion *solidRoughness =
      static_cast<G4MultiUnion *>(fSurfaceGenerator.GetSolid());

  solidRoughness->AddNode(*solidBasis, basisTransform);

  //
  // Voxelize
  //
  //
  Surface::G4Voxelizer_Green &voxelizer =
      (Surface::G4Voxelizer_Green &)solidRoughness->GetVoxels();
  voxelizer.SetMaxBoundary(10, 10, 2);
  voxelizer.Voxelize(solidRoughness);

  G4LogicalVolume *logicRoughness =
      new G4LogicalVolume(solidRoughness, roughness_mat, "Roughness");
  new G4PVPlacement(NULL, G4ThreeVector(), logicRoughness, "Roughness",
                    logicSubworld, 0, 0, checkOverlaps);

  fSurfaceGenerator.SetSurfaceTransformation(subworldPlacement);
  Surface::FacetStore &store = Surface::Locator::GetFacetStore();
  store = *fSurfaceGenerator.GetFacetStore();
  //
  // always return the physical World
  //
  return physWorld;
}
