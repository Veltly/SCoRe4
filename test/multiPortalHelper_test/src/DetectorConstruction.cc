/**
 * @brief Detector construction for testing.
 * @details Instance a Portal-Subworld setup using the MultiportalHelper class.
 * Places a multiportal with 200x200 subworld gridpoints and three sample subworlds A, B, C.
 * Density for subworlds is 0.3, 0.5, 0.2.
 * @author C.Gruener
 * @date 24.05.21
 * @file DetectorConstruction.cc
 */

#include "DetectorConstruction.hh"

#include "../../../src/Portal/include/MultipleSubworld.hh"
#include "../../../src/Service/include/MultiportalHelper.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <CLHEP/Geometry/Transform3D.h>
#include <G4UserLimits.hh>

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), fScoringVolume(nullptr) {}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::Construct() {
  // Get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  G4bool checkOverlaps = false;

  // define simulation domain "world"
  const G4double world_sizeXY = 1 * m;
  const G4double world_sizeZ = 1 * m;
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

  //placement of Subworlds A, B, C
  //placement of subworld such that simulated particles cannot reach them from outside
  const G4ThreeVector placementA{5 * cm, 5 * cm, 22 * cm};
  const G4ThreeVector placementB{5 * cm, 5 * cm, 20 * cm};
  const G4ThreeVector placementC{5 * cm, 5 * cm, 18 * cm};
  const G4Transform3D trafoA{G4RotationMatrix(), placementA};
  const G4Transform3D trafoB{G4RotationMatrix(), placementB};
  const G4Transform3D trafoC{G4RotationMatrix(), placementC};

  //placement of Portal
  const G4ThreeVector placementPortal{0 * cm, 0 * cm, 10 * cm};
  const G4Transform3D trafoPortal{G4RotationMatrix(), placementPortal};

  //material of subworld
  G4Material *subworldMaterial = nist->FindOrBuildMaterial("G4_Si");

  //instantiate the helper class for portal - subworld setup
  Surface::MultiportalHelper helper("Helper",5);

  //set size of portal
  helper.SetDxPortal(10 * cm);
  helper.SetDyPortal(10 * cm);
  helper.SetDzPortal(0.5 * cm);

  //set size of single subworld
  helper.SetDxSub(0.5 * mm);
  helper.SetDySub(0.5 * mm);
  helper.SetDzSub(0.5 * cm);

  //set the number of subworlds in direction
  helper.SetNxSub(200);
  helper.SetNySub(200);

  //add the placement of subworlds
  helper.AddSubworldPlacement(trafoA);
  helper.AddSubworldPlacement(trafoB);
  helper.AddSubworldPlacement(trafoC);

  //add density of subworlds
  helper.AddSubworldDensity(0.3);
  helper.AddSubworldDensity(0.5);
  helper.AddSubworldDensity(0.2);

  //set necessary information for helper class
  helper.SetMotherVolume(logicWorld);
  helper.SetPortalPlacement(trafoPortal);
  helper.SetSubworldMaterial(subworldMaterial);
  helper.SetNDifferentSubworlds(3);
  helper.SetPortalName("Portal");

  //generate portal-subworld setup
  helper.Generate();

  //get subworlds to set StepLimit
  Surface::MultipleSubworld *subA = helper.GetSubworld(0);
  Surface::MultipleSubworld *subB = helper.GetSubworld(1);
  Surface::MultipleSubworld *subC = helper.GetSubworld(2);

  G4double maxStepsize = 100 * mm;
  G4double maxStepsize_subworld = 100. * mm;

  auto *limit = new G4UserLimits(maxStepsize);
  auto *limit_subworld = new G4UserLimits(maxStepsize_subworld);
  logicWorld->SetUserLimits(limit);

  subA->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  subB->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  subC->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);

  //   always return the physical World
  return physWorld;
}
