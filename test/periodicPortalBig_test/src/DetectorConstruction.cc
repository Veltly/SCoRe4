// Author C.Gruener
// Date 24-05-21
// File DetectorConstruction

#include "DetectorConstruction.hh"

#include "../../../src/Portal/include/PeriodicPortal.hh"
#include "../../../src/Portal/include/PortalStore.hh"
#include "../../../src/Service/include/Locator.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Portal/include/VPortal.hh"
#include <G4MultiUnion.hh>
#include <G4Region.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4UserLimits.hh>
#include <cfloat>

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
  // G4double box_sizeXY = 2 * cm;
  // G4double box_sizeZ = 1 * cm;
  // G4Material *box_mat = nist->FindOrBuildMaterial("G4_Si");
  // G4Box *solidBox = new G4Box("TestBox", // its name
  //                            0.5 * box_sizeXY, 0.5 * box_sizeXY,
  //                            0.5 * box_sizeZ); // its size

  // G4LogicalVolume *logicBox = new G4LogicalVolume(solidBox,   // its solid
  //                                                 box_mat,    // its material
  //                                                 "TestBox"); // its name

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

  G4double subworld_sizeXY = 0.1 * mm;
  G4double subworld_sizeZ = 1 * mm;

  G4double subworldTrigger_sizeXY = subworld_sizeXY * 1.1;
  G4double subworldTrigger_sizeZ = subworld_sizeZ * 1.1;
  G4ThreeVector subworldTriggerPlacement{5 * cm, 5 * cm, 20 * cm};
  G4Material *subworldTrigger_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box *solidSubworldTrigger =
      new G4Box("SubworldTrigger", // its name
                0.5 * subworldTrigger_sizeXY, 0.5 * subworldTrigger_sizeXY,
                0.5 * subworldTrigger_sizeZ); // its size

  G4LogicalVolume *logicSubworldTrigger =
      new G4LogicalVolume(solidSubworldTrigger, // its solid
                          subworldTrigger_mat,  // its material
                          "SubworldTrigger");   // its name

  G4VPhysicalVolume *physSubworldTrigger =
      new G4PVPlacement(0,                        // no rotation
                        subworldTriggerPlacement, //
                        logicSubworldTrigger,     // its logical volume
                        "SubworldTrigger",        // its name
                        logicWorld,               // its mother  volume
                        false,                    // no boolean operation
                        0,                        // copy number
                        checkOverlaps);           // overlaps checking

  G4ThreeVector subworldPlacement{0, 0, 0};
  G4Material *subworld_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box *solidSubworld = new G4Box("Subworld", // its name
                                   0.5 * subworld_sizeXY, 0.5 * subworld_sizeXY,
                                   0.5 * subworld_sizeZ); // its size

  G4LogicalVolume *logicSubworld =
      new G4LogicalVolume(solidSubworld, // its solid
                          subworld_mat,  // its material
                          "Subworld");   // its name

  G4VPhysicalVolume *physSubworld =
      new G4PVPlacement(0,                    // no rotation
                        subworldPlacement,    //
                        logicSubworld,        // its logical volume
                        "Subworld",           // its name
                        logicSubworldTrigger, // its mother  volume
                        false,                // no boolean operation
                        0,                    // copy number
                        checkOverlaps);       // overlaps checking

  G4double testobject_XY = subworld_sizeXY * 0.2;
  G4double testobject_Z = subworld_sizeZ * 0.2;
  G4ThreeVector testobjectPlacement{0, 0, 0};
  G4Material *testobject_mat = nist->FindOrBuildMaterial("G4_Si");
  G4Box *solidTestobject = new G4Box("Testobject", 0.5 * testobject_XY,
                                     0.5 * testobject_XY, 0.5 * testobject_Z);

  G4LogicalVolume *logicTestobject =
      new G4LogicalVolume(solidTestobject, testobject_mat, "Testobject");

  new G4PVPlacement(0, testobjectPlacement, logicTestobject,
                    "TestobjectInsideMiddle", logicSubworld, false, 0,
                    checkOverlaps);
  G4ThreeVector testobjectPlacementInsideUpper{0, 0, 0};
  testobjectPlacementInsideUpper.setZ((subworld_sizeZ - testobject_Z) / 2.);
  new G4PVPlacement(0, testobjectPlacementInsideUpper, logicTestobject,
                    "TestobjectInsideUpper", logicSubworld, false, 0,
                    checkOverlaps);
  G4ThreeVector testobjectPlacementInsideLower{0, 0, 0};
  testobjectPlacementInsideLower.setZ(-(subworld_sizeZ - testobject_Z) / 2.);
  new G4PVPlacement(0, testobjectPlacementInsideLower, logicTestobject,
                    "TestobjectInsideLower", logicSubworld, false, 0,
                    checkOverlaps);
  //
  // Portal entrance
  //
  //

  G4double portal_sizeXY = subworld_sizeXY * 1000;
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

  G4double testobjectB_XY = portal_sizeXY;
  G4double testobjectB_Z = portal_sizeZ;
  G4ThreeVector testobjectBPlacementLower = portalPlacement;
  testobjectBPlacementLower.setZ(testobjectBPlacementLower.z() -
                                 ((testobjectB_Z + portal_sizeZ) / 2.));
  G4ThreeVector testobjectBPlacementUpper = portalPlacement;
  testobjectBPlacementUpper.setZ(testobjectBPlacementUpper.z() +
                                 ((testobjectB_Z + portal_sizeZ) / 2.));
  G4Material *testobjectB_mat = nist->FindOrBuildMaterial("G4_Si");
  G4Box *solidTestobjectB =
      new G4Box("TestobjectOutside", 0.5 * testobjectB_XY, 0.5 * testobjectB_XY,
                0.5 * testobjectB_Z);

  G4LogicalVolume *logicTestobjectB = new G4LogicalVolume(
      solidTestobjectB, testobjectB_mat, "TestobjectOutside");

  new G4PVPlacement(0, testobjectBPlacementLower, logicTestobjectB,
                    "TestobjectOutsideLower", logicWorld, false, 0,
                    checkOverlaps);

  new G4PVPlacement(0, testobjectBPlacementUpper, logicTestobjectB,
                    "TestobjectOutsideUpper", logicWorld, false, 0,
                    checkOverlaps);

  Surface::PortalStore &portalStore = Surface::Locator::GetPortalStore();
  Surface::PeriodicPortal *portalEntrance =
      new Surface::PeriodicPortal("Entrance", physPortal, portalPlacement, 3);
  Surface::PeriodicPortal *portalSubworld = new Surface::PeriodicPortal(
      "Subworld", physSubworld, subworldTriggerPlacement, 3);
  // set trigger for portals
  portalEntrance->SetTrigger(physPortal);
  portalSubworld->SetTrigger(physSubworldTrigger);
  // link portals
  portalEntrance->SetOtherPortal(portalSubworld);
  portalSubworld->SetOtherPortal(portalEntrance);
  portalEntrance->SetAsPortal();
  portalSubworld->SetGrid(1000, 2000);
  // add them to the store
  portalStore.push_back(portalEntrance);
  portalStore.push_back(portalSubworld);

  // set StepLimit

  G4double maxStepsize = 0.1 * mm;
  G4double maxStepsize_portal = 0.1 * um;
  G4UserLimits *limit = new G4UserLimits(maxStepsize);
  G4UserLimits *limit_portal = new G4UserLimits(maxStepsize_portal);
  logicWorld->SetUserLimits(limit);
  logicSubworld->SetUserLimits(limit_portal);
  logicPortal->SetUserLimits(limit);
  logicTestobject->SetUserLimits(limit_portal);
  logicTestobjectB->SetUserLimits(limit_portal);
  //
  // always return the physical World
  //
  return physWorld;
}
