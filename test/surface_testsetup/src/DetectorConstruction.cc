/**
 * @brief detector construction for generating a surface
 * @file DetectorConstruction.cc
 * @date 2024-05-21
 */

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
      fScoringVolume(nullptr),
      fSurfaceGenerator(Surface::SurfaceGenerator("Surface_Testsetup")) {}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::Construct() {
  //get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  G4bool checkOverlaps = false;

  //create world
  G4double world_sizeXY = 1 * m;
  G4double world_sizeZ = 1 * m;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto *solidWorld = new G4Box("World",  // its name
                                0.5 * world_sizeXY, 0.5 * world_sizeXY,
                                0.5 * world_sizeZ);  // its size

  auto *logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                         world_mat, // its material
                                         "World");     // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(nullptr,                // no rotation
                        G4ThreeVector(),  // at (0,0,0)
                        logicWorld,       // its logical volume
                        "World",          // its name
                        nullptr,                // its mother volume
                        false,            // no boolean operation
                        0,                // copy number
                        checkOverlaps);   // overlaps checking

  //subworld to place the patch of Rough surface
  G4double subworld_sizeXY = 1 * cm;
  G4double subworld_sizeZ = 1 * cm;
  G4ThreeVector subworldPlacement{0 * cm, 0 * cm, 0 * cm};
  G4Material *subworld_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto *solidSubworld = new G4Box("Subworld",  // its name
                                   0.5 * subworld_sizeXY, 0.5 * subworld_sizeXY,
                                   0.5 * subworld_sizeZ);  // its size

  auto *logicSubworld =
      new G4LogicalVolume(solidSubworld,  // its solid
                          subworld_mat,   // its material
                          "Subworld");    // its name

  new G4PVPlacement(nullptr,                  // no rotation
                    subworldPlacement,  //
                    logicSubworld,      // its logical volume
                    "Subworld",         // its name
                    logicWorld,         // its mother volume
                    false,              // no boolean operation
                    0,                  // copy number
                    checkOverlaps);     // overlaps checking


  // generation of a patch of rough surface
  G4Material *roughness_mat = nist->FindOrBuildMaterial("G4_Si");

  // generation of a basis
  G4double basis_sizeXY = 2 * mm;
  G4double basis_sizeZ = 1 * mm;
  auto *solidBasis = new G4Box("Basis", 0.5 * basis_sizeXY, 0.5 * basis_sizeXY,
                                0.5 * basis_sizeZ);

  G4ThreeVector basisPlacement{0., 0., -basis_sizeZ * 0.5};
  G4RotationMatrix basisRotation{G4RotationMatrix()};
  G4Transform3D basisTransform(basisRotation, basisPlacement);

  // generation of roughness using the describer of the surface generator class
  Surface::Describer &describer = fSurfaceGenerator.GetDescriber();
  describer.SetSpikeform(Surface::Describer::SpikeShape::UniformPyramid); // set the spike-form (there are predefined spike-forms)
  describer.SetNrSpike_X(2);  // number of spikes in x-direction
  describer.SetNrSpike_Y(2);  // number of spikes in y-direction
  describer.SetSpikeWidth_X(0.5 * mm);  // x-width of spike
  describer.SetSpikeWidth_Y(0.5 * mm);  // y-width of spike
  describer.SetMeanHeight(0.5 * mm);  // mean height of spike
  describer.SetNLayer(1); // number of layers used for a spike
  describer.SetHeightDeviation(0.1 * mm); // set sigma for deviation from mean height

  // generate rough surface
  fSurfaceGenerator.GenerateSurface();
  auto *solidRoughness =
      dynamic_cast<G4MultiUnion *>(fSurfaceGenerator.GetSolid());

  solidRoughness->AddNode(*solidBasis, basisTransform);

 // Voxelize
  // ------------------------------------------------------
  auto &voxelizer =
      (Surface::G4Voxelizer_Green &)solidRoughness->GetVoxels(); // get voxels from roughness (multiunion)
  voxelizer.SetMaxBoundary(10, 10, 2); // maximal number of boundaries for voxelization
  voxelizer.Voxelize(solidRoughness); // voxelize
  // ------------------------------------------------------

  //place roughness in the world
  auto *logicRoughness =
      new G4LogicalVolume(solidRoughness, roughness_mat, "Roughness");
  new G4PVPlacement(nullptr, G4ThreeVector(), logicRoughness, "Roughness",
                    logicSubworld, false, 0, checkOverlaps);

  // pass the facets from surface generator the general facet store
  fSurfaceGenerator.SetSurfaceTransformation(subworldPlacement);
  Surface::FacetStore &store = Surface::Locator::GetFacetStore();
  store = *fSurfaceGenerator.GetFacetStore();

  //return the physical World
  return physWorld;
}
