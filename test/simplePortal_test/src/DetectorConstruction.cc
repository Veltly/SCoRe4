/**
 * @brief Construct test setup for a simple portal represented by just one volume.
 * @author C.Gruener
 * @date 24-07-17
 */

#include "DetectorConstruction.hh"

#include <G4ThreeVector.hh>
#include <G4UserLimits.hh>

#include "../../../src/Portal/include/PortalStore.hh"
#include "../../../src/Portal/include/SimplePortal.hh"
#include "../../../src/Service/include/Locator.hh"
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
  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  G4bool checkOverlaps{false};

  //
  // create World
  //
  G4double world_sizeXY{1 * m};
  G4double world_sizeZ{1 * m};
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


  //
  // Subworld to port into
  //
  //

  G4double subworld_sizeXY{1 * cm}; //size of subworld
  G4double subworld_sizeZ{1 * cm}; //size of subworld

  G4double subworldTrigger_sizeXY{subworld_sizeXY * 1.1}; //trigger size
  G4double subworldTrigger_sizeZ{subworld_sizeZ * 1.1}; //trigger size
  G4ThreeVector subworldTriggerPlacement{5 * cm, 5 * cm, 20 * cm}; //placement of trigger
  G4Material *subworldTrigger_mat = nist->FindOrBuildMaterial("G4_AIR"); //material of trigger is not necessary to be G4_AIR
  auto *solidSubworldTrigger =
      new G4Box("SubworldTrigger",  // its name
                0.5 * subworldTrigger_sizeXY, 0.5 * subworldTrigger_sizeXY,
                0.5 * subworldTrigger_sizeZ);  // its size

  auto *logicSubworldTrigger =
      new G4LogicalVolume(solidSubworldTrigger,  // its solid
                          subworldTrigger_mat,   // its material
                          "SubworldTrigger");    // its name

  G4VPhysicalVolume *physSubworldTrigger =
      new G4PVPlacement(nullptr,                       //no rotation
                        subworldTriggerPlacement,      //its location
                        logicSubworldTrigger,  // its logical volume
                        "SubworldTrigger",           // its name
                        logicWorld,            // its mother volume
                        false,                       // no boolean operation
                        0,                         // copy number
                        checkOverlaps);            // overlaps checking

  G4ThreeVector subworldPlacement{0, 0, 0}; //subworld location
  G4Material *subworld_mat = nist->FindOrBuildMaterial("G4_AIR");
  auto *solidSubworld = new G4Box("Subworld",  // its name
                                   0.5 * subworld_sizeXY, 0.5 * subworld_sizeXY,
                                   0.5 * subworld_sizeZ);  // its size

  auto *logicSubworld =
      new G4LogicalVolume(solidSubworld,  // its solid
                          subworld_mat,   // its material
                          "Subworld");    // its name

  G4VPhysicalVolume *physSubworld =
      new G4PVPlacement(nullptr,                     // no rotation
                        subworldPlacement,     //
                        logicSubworld,         // its logical volume
                        "Subworld",            // its name
                        logicSubworldTrigger,  // its mother volume
                        false,                 // no boolean operation
                        0,                     // copy number
                        checkOverlaps);        // overlaps checking

  //
  // create testobject and place three times in subworld
  //

  G4double testobject_XY{subworld_sizeXY * 0.2};
  G4double testobject_Z{subworld_sizeZ * 0.2};
  G4ThreeVector testobjectPlacement{0, 0, 0};
  G4Material *testobject_mat = nist->FindOrBuildMaterial("G4_Si");
  auto *solidTestobject = new G4Box("Testobject", 0.5 * testobject_XY,
                                     0.5 * testobject_XY, 0.5 * testobject_Z);

  auto *logicTestobject =
      new G4LogicalVolume(solidTestobject, testobject_mat, "Testobject");

  new G4PVPlacement(nullptr, testobjectPlacement, logicTestobject,
                    "TestobjectInsideMiddle", logicSubworld, false, 0,
                    checkOverlaps);
  G4ThreeVector testobjectPlacementInsideUpper{0, 0, 0};
  testobjectPlacementInsideUpper.setZ((subworld_sizeZ - testobject_Z) / 2.);
  new G4PVPlacement(nullptr, testobjectPlacementInsideUpper, logicTestobject,
                    "TestobjectInsideUpper", logicSubworld, false, 0,
                    checkOverlaps);
  G4ThreeVector testobjectPlacementInsideLower{0, 0, 0};
  testobjectPlacementInsideLower.setZ(-(subworld_sizeZ - testobject_Z) / 2.);
  new G4PVPlacement(nullptr, testobjectPlacementInsideLower, logicTestobject,
                    "TestobjectInsideLower", logicSubworld, false, 0,
                    checkOverlaps);
  //
  // create portal entrance
  //

  G4double portal_sizeXY{subworld_sizeXY}; //size portal
  G4double portal_sizeZ{2 * subworld_sizeZ};//size portal
  G4ThreeVector portalPlacement{0 * cm, 0 * cm, 10 * cm};//placement portal
  G4Material *portal_mat = nist->FindOrBuildMaterial("G4_AIR");
  auto *solidPortal = new G4Box("Portal", 0.5 * portal_sizeXY,
                                 0.5 * portal_sizeXY, 0.5 * portal_sizeZ);
  auto *logicPortal =
      new G4LogicalVolume(solidPortal, portal_mat, "Portal");

  G4VPhysicalVolume *physPortal =
      new G4PVPlacement(nullptr, portalPlacement, logicPortal, "Portal", logicWorld,
                        false, 0, checkOverlaps);
  //
  // Add both volumes to portal Store
  //

  G4double testobjectB_XY{portal_sizeXY};
  G4double testobjectB_Z{portal_sizeZ};
  G4ThreeVector testobjectBPlacementLower{portalPlacement};
  testobjectBPlacementLower.setZ(testobjectBPlacementLower.z() -
                                 ((testobjectB_Z + portal_sizeZ) / 2.));
  G4ThreeVector testobjectBPlacementUpper{portalPlacement};
  testobjectBPlacementUpper.setZ(testobjectBPlacementUpper.z() +
                                 ((testobjectB_Z + portal_sizeZ) / 2.));
  G4Material *testobjectB_mat = nist->FindOrBuildMaterial("G4_Si");
  auto *solidTestobjectB =
      new G4Box("TestobjectOutside", 0.5 * testobjectB_XY, 0.5 * testobjectB_XY,
                0.5 * testobjectB_Z);

  auto *logicTestobjectB = new G4LogicalVolume(
      solidTestobjectB, testobjectB_mat, "TestobjectOutside");

  new G4PVPlacement(nullptr, testobjectBPlacementLower, logicTestobjectB,
                    "TestobjectOutsideLower", logicWorld, false, 0,
                    checkOverlaps);

  new G4PVPlacement(nullptr, testobjectBPlacementUpper, logicTestobjectB,
                    "TestobjectOutsideUpper", logicWorld, false, 0,
                    checkOverlaps);

  Surface::PortalStore &portalStore = Surface::Locator::GetPortalStore(); //get singleton portal store
  auto *portalEntrance =
      new Surface::SimplePortal("Entrance", physPortal, portalPlacement, 3);
  auto *portalSubworld = new Surface::SimplePortal(
      "Subworld", physSubworld, subworldTriggerPlacement, 3);

  portalEntrance->SetTrigger(physPortal); //set trigger for portation
  portalSubworld->SetTrigger(physSubworldTrigger); //set trigger for portation

  portalEntrance->SetOtherPortal(portalSubworld); //link portals
  portalSubworld->SetOtherPortal(portalEntrance); //link portals

  portalStore.push_back(portalEntrance); //add entrance to portal store
  portalStore.push_back(portalSubworld); //add subworld to portal store


  //
  // set step limit
  //

  G4double maxStepsize{0.1 * mm};
  auto *limit = new G4UserLimits(maxStepsize);
  logicWorld->SetUserLimits(limit);
  logicSubworld->SetUserLimits(limit);
  logicPortal->SetUserLimits(limit);
  logicTestobject->SetUserLimits(limit);
  logicTestobjectB->SetUserLimits(limit);

  return physWorld; //return physical world
}
