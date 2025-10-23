/**
 * @brief detector construction for generating a surface from gdml file
 * @file DetectorConstruction.cc
 * @date 2025-10-22
 */

#include "../../test_surface_placement/include/DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "Surface/LogicalSurface.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fScoringVolume(nullptr) {}

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


  // generation of a patch of rough surface
  G4Material *roughness_mat = nist->FindOrBuildMaterial("G4_Si");

  Surface::LogicalSurface surface{"SurfaceLV",
                                  "test_small.gdml",
                                  5, 7,
                                  roughness_mat,
                                  world_mat,
                                  Surface::VerboseLevel::DebugInfo};

  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    surface.get_logical_handle(),
                    "Surface",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);



  //return the physical World
  return physWorld;
}
