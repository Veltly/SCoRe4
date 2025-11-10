/**
* @brief Implementation of detector construction class for generating a surface
* @file DetectorConstruction.hh
* @date 2025-10-27
*/

#include "DetectorConstruction.hh"

#include "BoundarySensitiveDetector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4UserLimits.hh"
#include "SensitiveDetector.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      f_portal_helper("PortalHelper", Surface::VerboseLevel::Info),
      f_surface_helper("RoughnessHelper", Surface::VerboseLevel::Info){}

DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume *DetectorConstruction::Construct() {
  //get nist material manager
  G4NistManager *nist = G4NistManager::Instance();

  const G4bool check_overlaps = true;
  //create world

  const G4double world_size_xy = 1 * m;
  const G4double world_size_z = 1 * m;


  G4Material *world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  auto *solid_world = new G4Box("WorldSolid",  // its name
                               0.5 * world_size_xy, 0.5 * world_size_xy,
                               0.5 * world_size_z);  // its size

  auto *logic_world = new G4LogicalVolume(solid_world,  // its solid
                                         world_mat, // its material
                                         "WorldLV");     // its name

  G4VPhysicalVolume *phys_world =
      new G4PVPlacement(nullptr,                // no rotation
                        G4ThreeVector(),  // at (0,0,0)
                        logic_world,       // its logical volume
                        "World",          // its name
                        nullptr,                // its mother volume
                        false,            // no boolean operation
                        0,                // copy number
                        check_overlaps);   // overlaps checking

  //create envelope
  const G4double shell_thickness = 10*cm;

  auto *solid_envelope = new G4Box("EnvelopeSolid",
                             0.5 * (world_size_xy - shell_thickness),
                             0.5 * (world_size_xy - shell_thickness),
                             0.5 * (world_size_z - shell_thickness));

  auto *logic_envelope = new G4LogicalVolume(solid_envelope, world_mat, "EnvelopeLV");

  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    logic_envelope,
                    "Envelope",
                    logic_world,
                    false,
                    0, check_overlaps);

  //create outer scoring detector
  auto *solid_shell = new G4SubtractionSolid("ShellSolid", solid_world, solid_envelope);

  auto *logic_shell = new G4LogicalVolume(solid_shell, world_mat, "ShellLV");

  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    logic_shell,
                    "Shell", logic_world,
                    false,
                    0, check_overlaps);


  //Generate cube
  G4Material *cube_material = nist->FindOrBuildMaterial("G4_Si");

  const double cube_x = 3. * cm;
  const double cube_y = 3. * cm;
  const double cube_z = 3. * cm;

  auto *solid_cube = new G4Box("CubeSolid",
                              cube_x, cube_y, cube_z);

  auto *logic_cube = new G4LogicalVolume(solid_cube,
                                         cube_material,
                                        "CubeLV");

  new G4PVPlacement(nullptr,
                    G4ThreeVector(),
                    logic_cube,
                    "Cube",
                    logic_envelope,
                    false,
                    0, check_overlaps);


  // set portal parameters
  const G4ThreeVector placement_subworld{5 * cm, 5 * cm, 22 * cm};
  const G4Transform3D trafo{G4RotationMatrix(), placement_subworld};
  const G4ThreeVector placementPortal{0 * cm, 0 * cm,
                                      cube_z + f_portal_helper.GetPortalDz()};
  const G4Transform3D trafoPortal{G4RotationMatrix(), placementPortal};

  f_portal_helper.AddSubworldPlacement(trafo);
  f_portal_helper.AddSubworldDensity(1.);
  f_portal_helper.SetMotherVolume(logic_world);
  f_portal_helper.SetPortalPlacement(trafoPortal);
  f_portal_helper.SetNDifferentSubworlds(1);
  f_portal_helper.SetPortalName("Portal");

  // generate surface
  f_surface_helper.Generate();


  // add surface to portal_helper
  const G4Transform3D trafo_surface{
      G4RotationMatrix(),
      G4ThreeVector(0., 0.,
                    +f_surface_helper.GetBasisHeight() * 2 - f_portal_helper.GetPortalDz())};

  f_portal_helper.AddRoughness(f_surface_helper.LogicRoughness(), trafo_surface,
                             f_surface_helper.FacetStore());

  // generate portal
  f_portal_helper.Generate();

  CheckValues();

  Surface::MultipleSubworld *sub = f_portal_helper.GetSubworld(0);
//
  G4double maxStepsize = 1 * CLHEP::mm;
  G4double maxStepsize_subworld = 100. * CLHEP::nm;
  auto *limit = new G4UserLimits(maxStepsize);
  auto *limit_subworld = new G4UserLimits(maxStepsize_subworld);
  logic_world->SetUserLimits(limit);
  sub->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);

  //return the physical World
  return phys_world;
}

void DetectorConstruction::ConstructSDandField() {
  auto sdManager = G4SDManager::GetSDMpointer();
  auto sensitive_detector = new SensitiveDetector("SensitiveDetector");
  sdManager->AddNewDetector(sensitive_detector);
  SetSensitiveDetector("CubeLV", sensitive_detector);
  SetSensitiveDetector(f_surface_helper.GetRoughnessLogicalVolumeName(), sensitive_detector);

  auto boundary_detector = new BoundarySensitiveDetector("BoundaryDetector");
  sdManager->AddNewDetector(boundary_detector);
  SetSensitiveDetector("ShellLV", boundary_detector,true);
}

void DetectorConstruction::CheckValues() const {
  auto isSame = [](const G4double val_a, const G4double val_b) {
    const G4double numeric_limit = std::numeric_limits<G4double>::epsilon() * 10;
    return std::fabs(val_a - val_b) < numeric_limit;
  };

  const G4double sub_dx{f_portal_helper.GetSubworldDx()};
  const G4double sub_dy{f_portal_helper.GetSubworldDy()};
  const G4double sub_dz{f_portal_helper.GetSubworldDz()};

  const G4double spike_dx{f_surface_helper.GetSpikeDx()};
  const G4double spike_dy{f_surface_helper.GetSpikeDy()};
  const G4double spike_height{f_surface_helper.GetSpikeMeanHeight()};
  const G4int spike_nx{f_surface_helper.GetSpikeNx()};
  const G4int spike_ny{f_surface_helper.GetSpikeNy()};
  const G4double basis_dz{f_surface_helper.GetBasisHeight()};

  G4bool error{false};
  std::stringstream ss;
  ss << "\n";


  if (!isSame(spike_dx * spike_nx, sub_dx)) {
    ss << "Roughness:\n";
    ss << "Spikes do not fit in Subworld (X-Dimension)!\n";
    ss << "SpikeDx * SpikesNx != SubworldDx\n";
    ss << spike_dx / CLHEP::mm << " mm * " << spike_nx
       << " != " << sub_dx / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (!isSame(spike_dy * spike_ny, sub_dy)) {
    ss << "Roughness:\n";
    ss << "Spikes do not fit in Subworld (Y-Dimension)!\n";
    ss << "SpikeDy * SpikesNy != SubworldDy\n";
    ss << spike_dy / CLHEP::mm << " mm * " << spike_ny
       << " != " << sub_dy / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

//  if (!isSame(spike_height + basis_dz, sub_dz)) {
  if(spike_height + basis_dz > sub_dz){
    ss << "Roughness:\n";
    ss << "Spikes do not fit in Subworld (Z-Dimension)!\n";
    ss << "SpikeHeight + BasisHeight != SubworldDz\n";
    ss << spike_height / CLHEP::mm << " mm + " << basis_dz / CLHEP::mm
       << " mm != " << sub_dz / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (error) {
    std::stringstream stream;
    stream << "\n";
    stream << "**************************************************\n";
    stream << "*                                                *\n";
    stream << "*        Error while generating Detector!        *\n";
    stream << "*                                                *\n";
    stream << "**************************************************\n";

    ss << "\n";
    ss << "**************************************************\n";
    ss << "**************************************************\n";
    G4cout << stream.str() << ss.str() << G4endl;
    G4String msg = ss.str();
    G4Exception("DetectorConstruction::CheckValues()", "", FatalException,msg);
  }
}