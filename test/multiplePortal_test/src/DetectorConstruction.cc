// Author C.Gruener
// Date 24-05-21
// File DetectorConstruction

#include "DetectorConstruction.hh"

#include "../../../src/Portal/include/MultipleSubworld.hh"
#include "../../../src/Portal/include/PortalStore.hh"
#include "../../../src/Service/include/Locator.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Portal/include/SubworldGrid.hh"
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

  // Creating three Subworlds A, B ,C to port into
  //
  //

  G4double subworld_sizeXY = 0.1 * mm;
  G4double subworld_sizeZ = 1 * mm;

  G4double subworldTrigger_sizeXY = subworld_sizeXY * 1.1;
  G4double subworldTrigger_sizeZ = subworld_sizeZ * 1.1;

  // Subworld Trigger

  G4Material *subworldTrigger_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box *solidSubworldTrigger =
      new G4Box("SubworldTrigger", // its name
                0.5 * subworldTrigger_sizeXY, 0.5 * subworldTrigger_sizeXY,
                0.5 * subworldTrigger_sizeZ); // its size

  G4LogicalVolume *logicSubworldTriggerA =
      new G4LogicalVolume(solidSubworldTrigger, // its solid
                          subworldTrigger_mat,  // its material
                          "SubworldTriggerA");  // its name

  G4LogicalVolume *logicSubworldTriggerB =
      new G4LogicalVolume(solidSubworldTrigger, // its solid
                          subworldTrigger_mat,  // its material
                          "SubworldTriggerB");  // its name

  G4LogicalVolume *logicSubworldTriggerC =
      new G4LogicalVolume(solidSubworldTrigger, // its solid
                          subworldTrigger_mat,  // its material
                          "SubworldTriggerC");  // its name
  // Subworld

  G4Material *subworld_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Box *solidSubworld = new G4Box("Subworld", // its name
                                   0.5 * subworld_sizeXY, 0.5 * subworld_sizeXY,
                                   0.5 * subworld_sizeZ); // its size

  G4LogicalVolume *logicSubworld =
      new G4LogicalVolume(solidSubworld, // its solid
                          subworld_mat,  // its material
                          "Subworld");   // its name

  G4ThreeVector subworldTriggerPlacementA{5 * cm, 5 * cm, 22 * cm};
  G4ThreeVector subworldTriggerPlacementB{5 * cm, 5 * cm, 20 * cm};
  G4ThreeVector subworldTriggerPlacementC{5 * cm, 5 * cm, 18 * cm};

  G4VPhysicalVolume *physSubworldTriggerA =
      new G4PVPlacement(0,                         // no rotation
                        subworldTriggerPlacementA, //
                        logicSubworldTriggerA,     // its logical volume
                        "SubworldTriggerA",        // its name
                        logicWorld,                // its mother  volume
                        false,                     // no boolean operation
                        0,                         // copy number
                        checkOverlaps);            // overlaps checking

  G4VPhysicalVolume *physSubworldTriggerB =
      new G4PVPlacement(0,                         // no rotation
                        subworldTriggerPlacementB, //
                        logicSubworldTriggerB,     // its logical volume
                        "SubworldTriggerB",        // its name
                        logicWorld,                // its mother  volume
                        false,                     // no boolean operation
                        0,                         // copy number
                        checkOverlaps);            // overlaps checking

  G4VPhysicalVolume *physSubworldTriggerC =
      new G4PVPlacement(0,                         // no rotation
                        subworldTriggerPlacementC, //
                        logicSubworldTriggerC,     // its logical volume
                        "SubworldTriggerC",        // its name
                        logicWorld,                // its mother  volume
                        false,                     // no boolean operation
                        0,                         // copy number
                        checkOverlaps);            // overlaps checking

  G4ThreeVector subworldPlacement{0, 0, 0};

  G4VPhysicalVolume *physSubworldA =
      new G4PVPlacement(0,                     // no rotation
                        subworldPlacement,     //
                        logicSubworld,         // its logical volume
                        "Subworld-A",          // its name
                        logicSubworldTriggerA, // its mother  volume
                        false,                 // no boolean operation
                        0,                     // copy number
                        checkOverlaps);        // overlaps checking
  G4VPhysicalVolume *physSubworldB =
      new G4PVPlacement(0,                     // no rotation
                        subworldPlacement,     //
                        logicSubworld,         // its logical volume
                        "Subworld-B",          // its name
                        logicSubworldTriggerB, // its mother  volume
                        false,                 // no boolean operation
                        0,                     // copy number
                        checkOverlaps);        // overlaps checking
  G4VPhysicalVolume *physSubworldC =
      new G4PVPlacement(0,                     // no rotation
                        subworldPlacement,     //
                        logicSubworld,         // its logical volume
                        "Subworld-C",          // its name
                        logicSubworldTriggerC, // its mother  volume
                        false,                 // no boolean operation
                        0,                     // copy number
                        checkOverlaps);        // overlaps checking

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
  // Add Portal and Subworlds to portal Store
  //
  //

  Surface::PortalStore &portalStore = Surface::Locator::GetPortalStore();
  Surface::MultipleSubworld *portalEntrance =
      new Surface::MultipleSubworld("Entrance", physPortal, portalPlacement, 3);

  Surface::MultipleSubworld *portalSubworldA = new Surface::MultipleSubworld(
      "Subworld-A", physSubworldA, subworldTriggerPlacementA, 3);
  Surface::MultipleSubworld *portalSubworldB = new Surface::MultipleSubworld(
      "Subworld-B", physSubworldB, subworldTriggerPlacementB, 3);
  Surface::MultipleSubworld *portalSubworldC = new Surface::MultipleSubworld(
      "Subworld-C", physSubworldC, subworldTriggerPlacementC, 3);

  // set trigger for portals
  portalEntrance->SetTrigger(physPortal);
  portalSubworldA->SetTrigger(physSubworldTriggerA);
  portalSubworldB->SetTrigger(physSubworldTriggerB);
  portalSubworldC->SetTrigger(physSubworldTriggerC);
  // link portals
  portalEntrance->SetAsPortal();
  portalEntrance->SetSubwordlEdge(subworld_sizeXY, subworld_sizeXY,
                                  subworld_sizeZ);
  portalEntrance->SetGrid(1000, 2000, 0);
  portalEntrance->SetOtherPortal(portalSubworldA);
  Surface::HelperFillSubworldGrid<Surface::MultipleSubworld> subworldHelper(0);
  subworldHelper.AddAvailableSubworld(portalSubworldA, 0.3);
  subworldHelper.AddAvailableSubworld(portalSubworldB, 0.5);
  subworldHelper.AddAvailableSubworld(portalSubworldC, 0.2);
  subworldHelper.FillGrid(portalEntrance->GetSubworldGrid());
  portalEntrance->GetSubworldGrid()->PrintUniqueSubworlds();
  portalEntrance->GetSubworldGrid()->PrintStatistic();
  portalEntrance->GetSubworldGrid()->PrintLegend();
  portalEntrance->GetSubworldGrid()->PrintGrid(0, 4, 0, 5);

  // add them to the store
  portalStore.push_back(portalEntrance);
  portalStore.push_back(portalSubworldA);
  portalStore.push_back(portalSubworldB);
  portalStore.push_back(portalSubworldC);

  // set StepLimit

  G4double maxStepsize = 0.1 * mm;
  G4double maxStepsize_subworld = 0.1 * um;
  G4UserLimits *limit = new G4UserLimits(maxStepsize);
  G4UserLimits *limit_subworld = new G4UserLimits(maxStepsize_subworld);
  logicWorld->SetUserLimits(limit);
  logicSubworld->SetUserLimits(limit_subworld);
  logicPortal->SetUserLimits(limit);
  //
  // always return the physical World
  //
  return physWorld;
}
