/**
* @brief Implementation of detector construction class for generating a surface
* @file DetectorConstruction.hh
* @date 2025-10-27
*/

#include "../../example_surface/include/DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "Surface/LogicalSurface.hh"
#include "Surface/SurfacePlacement.hh"
#include "G4SDManager.hh"
#include "SensitiveDetector.hh"
#include "BoundarySensitiveDetector.hh"
#include "G4SubtractionSolid.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fScoringVolume(nullptr) {}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::Construct() {
  //get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  const G4bool checkOverlaps = false;
  const Surface::VerboseLevel verboseLvl = Surface::VerboseLevel::Info;

  //create world

  const G4double world_sizeXY = 1 * m;
  const G4double world_sizeZ = 1 * m;


  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  auto *solidWorld = new G4Box("WorldSolid",  // its name
                               0.5 * world_sizeXY, 0.5 * world_sizeXY,
                               0.5 * world_sizeZ);  // its size

  auto *logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                         world_mat, // its material
                                         "WorldLV");     // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(nullptr,                // no rotation
                        G4ThreeVector(),  // at (0,0,0)
                        logicWorld,       // its logical volume
                        "World",          // its name
                        nullptr,                // its mother volume
                        false,            // no boolean operation
                        0,                // copy number
                        checkOverlaps);   // overlaps checking

  //create envelope
  const G4double shell_thickness = 10*cm;

  auto *solid_envelope = new G4Box("EnvelopeSolid",
                             0.5 * (world_sizeXY - shell_thickness),
                             0.5 * (world_sizeXY - shell_thickness),
                             0.5 * (world_sizeZ - shell_thickness));

  auto *logic_envelope = new G4LogicalVolume(solid_envelope, world_mat, "EnvelopeLV");

  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    logic_envelope,
                    "Envelope",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);

  //create outer scoring detector
  auto *solid_shell = new G4SubtractionSolid("ShellSolid", solidWorld, solid_envelope);

  auto *logic_shell = new G4LogicalVolume(solid_shell, world_mat, "ShellLV");

  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    logic_shell,
                    "Shell",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);


  //Generate cube
  G4Material *cubeMaterial = nist->FindOrBuildMaterial("G4_Si");

  const double cube_x = 3. * cm;
  const double cube_y = 3. * cm;
  const double cube_z = 3. * cm;

  auto *solidCube = new G4Box("CubeSolid",
                              cube_x, cube_y, cube_z);

  auto *logicCube = new G4LogicalVolume(solidCube,
                                        cubeMaterial,
                                        "CubeLV");

  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    logicCube,
                    "Cube",
                    logic_envelope,
                    false,
                    0,
                    checkOverlaps);

  //generating surface
  const G4String gdml_filename = "macros/test_small.gdml";

  auto *surface = new Surface::LogicalSurface{"SurfaceLV",
                                  gdml_filename,
                                  6, 6,
                                  cubeMaterial,
                                  world_mat,
                                  verboseLvl};

  //top
  const auto shift_to_zero = surface->get_shift_to_zero();
  const G4ThreeVector position_top{0,0,cube_z + shift_to_zero};
  auto *rotation_top = new G4RotationMatrix (0,0,0);
  Surface::SurfacePlacement(rotation_top,
                            position_top,
                            surface,
                            "Surface_top",
                            logic_envelope,
                            checkOverlaps,
                            true);


  //bottom
  const G4ThreeVector position_bottom{0,0,-cube_z - shift_to_zero};
  auto *rotation_bottom = new G4RotationMatrix (0,180 * degree,0);
  Surface::SurfacePlacement(rotation_bottom,
                            position_bottom,
                            surface,
                            "Surface_bottom",
                            logic_envelope,
                            checkOverlaps,
                            true);

  //side +x
  const G4ThreeVector position_plus_x{cube_x + shift_to_zero,0,0};
  auto *rotation_plus_x = new G4RotationMatrix (90*degree,90 * degree,0);
  Surface::SurfacePlacement(rotation_plus_x,
                            position_plus_x,
                            surface,
                            "Surface_plus_x",
                            logic_envelope,
                            checkOverlaps,
                            true);

  //side -x
  const G4ThreeVector position_minus_x{-cube_x - shift_to_zero,0,0};
  auto *rotation_minus_x = new G4RotationMatrix (270*degree,90 * degree,0);
  Surface::SurfacePlacement(rotation_minus_x,
                            position_minus_x,
                            surface,
                            "Surface_minus_x",
                            logic_envelope,
                            checkOverlaps,
                            true);

  //side +y
  const G4ThreeVector position_plus_y{0,cube_y + shift_to_zero,0};
  auto *rotation_plus_y = new G4RotationMatrix (180*degree,90 * degree,0);
  Surface::SurfacePlacement(rotation_plus_y,
                            position_plus_y,
                            surface,
                            "Surface_plus_y",
                            logic_envelope,
                            checkOverlaps,
                            true);

  //side -y
  const G4ThreeVector position_minus_y{0,-cube_y - shift_to_zero,0};
  auto *rotation_minus_y = new G4RotationMatrix (0,90 * degree,0);
  Surface::SurfacePlacement(rotation_minus_y,
                            position_minus_y,
                            surface,
                            "Surface_minus_y",
                            logic_envelope,
                            checkOverlaps,
                            true);
  surface->force_probability_generation();
//  surface->show_information();
//  surface->show_probability_information();
//  surface->show_placed_elements_information();


  //return the physical World
  return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
  auto sdManager = G4SDManager::GetSDMpointer();
  auto sensitive_detector = new SensitiveDetector("SensitiveDetector");
  sdManager->AddNewDetector(sensitive_detector);
  SetSensitiveDetector("CubeLV", sensitive_detector);
  SetSensitiveDetector("SurfaceLV", sensitive_detector);

  auto boundary_detector = new BoundarySensitiveDetector("BoundaryDetector");
  sdManager->AddNewDetector(boundary_detector);
  SetSensitiveDetector("ShellLV", boundary_detector,true);
}