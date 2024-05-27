// Author C.Gruener
// Date 24-05-21
// File DetectorConstruction

#include "DetectorConstruction.hh"

#include "../../../src/Portal/include/PortalStore.hh"
#include "../../../src/Portal/include/SimplePortal.hh"
#include "../../../src/Service/include/Locator.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Portal/include/VPortal.hh"
#include <G4MultiUnion.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>

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
  G4double world_sizeXY = 1 * m;
  G4double world_sizeZ = 1 * m;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box *solidWorld = new G4Box("World", // its name
                                0.5 * world_sizeXY, 0.5 * world_sizeXY,
                                0.5 * world_sizeZ); // its size

  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, // its solid
                                                    world_mat,  // its material
                                                    "World");   // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0,               // no rotation
                        G4ThreeVector(), // at (0,0,0)
                        logicWorld,      // its logical volume
                        "World",         // its name
                        0,               // its mother  volume
                        false,           // no boolean operation
                        0,               // copy number
                        checkOverlaps);  // overlaps checking

  //
  // Box
  //
  //
  G4double box_sizeXY = 2 * cm;
  G4double box_sizeZ = 1 * cm;
  G4Material *box_mat = nist->FindOrBuildMaterial("G4_Si");
  G4Box *solidBox = new G4Box("TestBox", // its name
                              0.5 * box_sizeXY, 0.5 * box_sizeXY,
                              0.5 * box_sizeZ); // its size

  G4LogicalVolume *logicBox = new G4LogicalVolume(solidBox,   // its solid
                                                  box_mat,    // its material
                                                  "TestBox"); // its name

  //  new G4PVPlacement(0,               // no rotation
  //                    G4ThreeVector(), // at (0,0,0)
  //                    logicBox,        // its logical volume
  //                    "TestBox",       // its name
  //                    logicWorld,      // its mother  volume
  //                    false,           // no boolean operation
  //                    0,               // copy number
  //                    checkOverlaps);  // overlaps checking

  //
  // Subworld to port into
  //
  //

  G4double subworld_sizeXY = 1 * cm;
  G4double subworld_sizeZ = 1 * cm;
  G4ThreeVector subworldPlacement{40 * cm, 40 * cm, 40 * cm};
  G4Material *subworld_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box *solidSubworld = new G4Box("Subworld", // its name
                                   0.5 * subworld_sizeXY, 0.5 * subworld_sizeXY,
                                   0.5 * subworld_sizeZ); // its size

  G4LogicalVolume *logicSubworld =
      new G4LogicalVolume(solidSubworld, // its solid
                          subworld_mat,  // its material
                          "Subworld");   // its name

  G4VPhysicalVolume *physSubworld =
      new G4PVPlacement(0,                 // no rotation
                        subworldPlacement, //
                        logicSubworld,     // its logical volume
                        "Subworld",        // its name
                        logicWorld,        // its mother  volume
                        false,             // no boolean operation
                        0,                 // copy number
                        checkOverlaps);    // overlaps checking

  //
  // Portal entrance
  //
  //

  G4double portal_sizeXY = subworld_sizeXY;
  G4double portal_sizeZ = subworld_sizeZ;
  G4ThreeVector portalPlacement{0 * cm, 0 * cm, 10 * cm};
  G4Material *portal_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box *solidPortal = new G4Box("Portal", 0.5 * portal_sizeXY,
                                 0.5 * portal_sizeXY, 0.5 * portal_sizeZ);
  G4LogicalVolume *logicPortal =
      new G4LogicalVolume(solidPortal, portal_mat, "Portal");

  G4VPhysicalVolume *physPortal =
      new G4PVPlacement(0, portalPlacement, logicPortal, "Portal", logicWorld,
                        false, 0, checkOverlaps);
  //
  // Add both volumes to portal Store
  //
  //

  Surface::PortalStore &portalStore = Surface::Locator::GetPortalStore();
  Surface::SimplePortal *portalEntrance =
      new Surface::SimplePortal("Entrance", physPortal, portalPlacement);
  Surface::SimplePortal *portalSubworld =
      new Surface::SimplePortal("Subworld", physSubworld, subworldPlacement);
  portalEntrance->SetOtherPortal(portalSubworld);
  portalSubworld->SetOtherPortal(portalEntrance);
  portalStore.push_back(portalEntrance);
  portalStore.push_back(portalSubworld);
  //
  // always return the physical World
  //
  return physWorld;
}
