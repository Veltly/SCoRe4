/**
 * @brief detector setup for test of surface shift
 * @file DetectorConstruction.cc
 * @author C.Gruener
 * @date 2024-05-21
 */

#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), fScoringVolume(nullptr) {}

DetectorConstruction::~DetectorConstruction() = default;

/**
 * @brief setup of two simple thin boxes placed parallel to each other with a small gap in between
 * @return physical world volume
 */
G4VPhysicalVolume *DetectorConstruction::Construct() {
  // get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  G4bool checkOverlaps = false;

  // setup of world volume
  G4double world_sizeXY = 1 * m;
  G4double world_sizeZ = 1 * m;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto *solidWorld = new G4Box("World", // its name
                                0.5 * world_sizeXY, 0.5 * world_sizeXY,
                                0.5 * world_sizeZ); // its size

  auto *logicWorld = new G4LogicalVolume(solidWorld, // its solid
                                         world_mat,  // its material
                                         "World");   // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(nullptr,               // no rotation
                        G4ThreeVector(), // at (0,0,0)
                        logicWorld,      // its logical volume
                        "World",         // its name
                        nullptr,               // its mother volume
                        false,           // no boolean operation
                        0,               // copy number
                        checkOverlaps);  // overlaps checking

  // setup of 2 simple thin boxs to place particles inside using the shift function and confine them to these boxes
  G4double box_sizeXY = 1 * cm;
  G4double box_sizeZ = 50 * nm;
  G4Material *box_mat = nist->FindOrBuildMaterial("G4_Si");
  auto *solidBox = new G4Box("TestBox", // its name
                              0.5 * box_sizeXY, 0.5 * box_sizeXY,
                              0.5 * box_sizeZ); // its size

  auto *logicBox = new G4LogicalVolume(solidBox,   // its solid
                                       box_mat,    // its material
                                       "TestBox"); // its name

  new G4PVPlacement(nullptr,                            // no rotation
                    G4ThreeVector(0, 0, 25 * nm), // at (0,0,25 nm)
                    logicBox,                     // its logical volume
                    "TestBox1",                   // its name
                    logicWorld,                   // its mother volume
                    false,                        // no boolean operation
                    0,                            // copy number
                    checkOverlaps);               // overlaps checking

  new G4PVPlacement(nullptr,                             // no rotation
                    G4ThreeVector(0, 0, 100 * nm), // at (0,0, 100 nm)
                    logicBox,                      // its logical volume
                    "TestBox2",                    // its name
                    logicWorld,                    // its mother volume
                    false,                         // no boolean operation
                    0,                             // copy number
                    checkOverlaps);                // overlaps checking

  // always return the physical World
  return physWorld;
}
