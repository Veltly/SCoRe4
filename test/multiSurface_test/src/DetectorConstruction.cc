// Author C.Gruener
// Date 24-05-21
// File DetectorConstruction

#include "DetectorConstruction.hh"

#include "../../../src/Portal/include/MultipleSubworld.hh"
#include "../../../src/Portal/include/PortalStore.hh"
#include "../../../src/Service/include/Locator.hh"
#include "../../../src/SurfaceGenerator/include/Describer.hh"
#include "../../../src/SurfaceGenerator/include/Generator.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Portal/include/VPortal.hh"
#include "Service/include/G4Voxelizer_Green.hh"
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

  // Creating three Subworlds A, B ,C to port into
  //
  //

  G4double subworld_sizeXY = 0.1 * mm;
  G4double subworld_sizeZ = 0.1 * mm;

  G4int griddim = 200;
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

  G4LogicalVolume *logicSubworldA =
      new G4LogicalVolume(solidSubworld, // its solid
                          subworld_mat,  // its material
                          "SubworldA");  // its name

  G4LogicalVolume *logicSubworldB =
      new G4LogicalVolume(solidSubworld, // its solid
                          subworld_mat,  // its material
                          "SubworldB");  // its name

  G4LogicalVolume *logicSubworldC =
      new G4LogicalVolume(solidSubworld, // its solid
                          subworld_mat,  // its material
                          "SubworldC");  // its name
  //
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
                        logicSubworldA,        // its logical volume
                        "SubworldA",           // its name
                        logicSubworldTriggerA, // its mother  volume
                        false,                 // no boolean operation
                        0,                     // copy number
                        checkOverlaps);        // overlaps checking
  G4VPhysicalVolume *physSubworldB =
      new G4PVPlacement(0,                     // no rotation
                        subworldPlacement,     //
                        logicSubworldB,        // its logical volume
                        "SubworldB",           // its name
                        logicSubworldTriggerB, // its mother  volume
                        false,                 // no boolean operation
                        0,                     // copy number
                        checkOverlaps);        // overlaps checking
  G4VPhysicalVolume *physSubworldC =
      new G4PVPlacement(0,                     // no rotation
                        subworldPlacement,     //
                        logicSubworldC,        // its logical volume
                        "SubworldC",           // its name
                        logicSubworldTriggerC, // its mother  volume
                        false,                 // no boolean operation
                        0,                     // copy number
                        checkOverlaps);        // overlaps checking

  // build rough surface for all subworlds
  //
  //

  G4double basis_sizeXY = subworld_sizeXY;
  G4double basis_sizeZ = subworld_sizeZ * 0.3;

  G4Material *surface_mat = nist->FindOrBuildMaterial("G4_Si");

  G4ThreeVector surfaceInSubworld{0, 0, -subworld_sizeZ * 0.5 + basis_sizeZ};
  G4ThreeVector basisPlacement{0, 0, -basis_sizeZ * 0.5};
  G4RotationMatrix basisRotation{G4RotationMatrix()};
  G4Transform3D basisTransform(basisRotation, basisPlacement);

  G4Box *solidBasis = new G4Box("Basis", 0.5 * basis_sizeXY, 0.5 * basis_sizeXY,
                                0.5 * basis_sizeZ);

  Surface::Describer &describerA = fGeneratorA.GetDescriber();
  Surface::Describer &describerB = fGeneratorB.GetDescriber();
  Surface::Describer &describerC = fGeneratorC.GetDescriber();

  describerA.SetNrSpike_X(50);
  describerA.SetNrSpike_Y(50);
  describerA.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerA.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerA.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.5);
  describerA.SetNLayer(1);
  describerA.SetHeightDeviation(0.0 * mm);
  describerA.SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);

  describerB.SetNrSpike_X(50);
  describerB.SetNrSpike_Y(50);
  describerB.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerB.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerB.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.3);
  describerB.SetNLayer(1);
  describerB.SetHeightDeviation(0.0 * mm);
  describerB.SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);

  describerC.SetNrSpike_X(50);
  describerC.SetNrSpike_Y(50);
  describerC.SetSpikeWidth_X(basis_sizeXY * 0.5 / 2.);
  describerC.SetSpikeWidth_Y(basis_sizeXY * 0.5 / 2.);
  describerC.SetMeanHeight((subworld_sizeZ - basis_sizeZ) * 0.1);
  describerC.SetNLayer(1);
  describerC.SetHeightDeviation(0.0 * mm);
  describerC.SetSpikeform(Surface::Describer::Spikeform::UniformPyramide);

  //
  // description of all surfaces finished, now generate them
  fGeneratorA.GenerateSurface();
  fGeneratorB.GenerateSurface();
  fGeneratorC.GenerateSurface();

  G4MultiUnion *solidSurfaceA =
      static_cast<G4MultiUnion *>(fGeneratorA.GetSolid());
  G4MultiUnion *solidSurfaceB =
      static_cast<G4MultiUnion *>(fGeneratorB.GetSolid());
  G4MultiUnion *solidSurfaceC =
      static_cast<G4MultiUnion *>(fGeneratorC.GetSolid());

  solidSurfaceA->AddNode(*solidBasis, basisTransform);
  solidSurfaceB->AddNode(*solidBasis, basisTransform);
  solidSurfaceC->AddNode(*solidBasis, basisTransform);

  // Voxelize all surfaces
  //

  Surface::G4Voxelizer_Green &voxelA =
      (Surface::G4Voxelizer_Green &)solidSurfaceA->GetVoxels();
  Surface::G4Voxelizer_Green &voxelB =
      (Surface::G4Voxelizer_Green &)solidSurfaceB->GetVoxels();
  Surface::G4Voxelizer_Green &voxelC =
      (Surface::G4Voxelizer_Green &)solidSurfaceC->GetVoxels();

  voxelA.SetMaxBoundary({10, 10, 2});
  voxelB.SetMaxBoundary({10, 10, 2});
  voxelC.SetMaxBoundary({10, 10, 2});

  voxelA.Voxelize(solidSurfaceA);
  voxelB.Voxelize(solidSurfaceB);
  voxelC.Voxelize(solidSurfaceC);

  G4ThreeVector facetStorePlacementA =
      subworldTriggerPlacementA + surfaceInSubworld;
  G4ThreeVector facetStorePlacementB =
      subworldTriggerPlacementB + surfaceInSubworld;
  G4ThreeVector facetStorePlacementC =
      subworldTriggerPlacementC + surfaceInSubworld;

  fGeneratorA.SetSurfaceTransformation(facetStorePlacementA);
  fGeneratorB.SetSurfaceTransformation(facetStorePlacementB);
  fGeneratorC.SetSurfaceTransformation(facetStorePlacementC);
  // generate logical volumes of roughness
  //
  G4LogicalVolume *logicSurfaceA =
      new G4LogicalVolume(solidSurfaceA, surface_mat, "SurfaceA");
  G4LogicalVolume *logicSurfaceB =
      new G4LogicalVolume(solidSurfaceB, surface_mat, "SurfaceB");
  G4LogicalVolume *logicSurfaceC =
      new G4LogicalVolume(solidSurfaceC, surface_mat, "SurfaceC");

  new G4PVPlacement(NULL, surfaceInSubworld, logicSurfaceA, "SurfaceA",
                    logicSubworldA, 0, 0, checkOverlaps);
  new G4PVPlacement(NULL, surfaceInSubworld, logicSurfaceB, "SurfaceB",
                    logicSubworldB, 0, 0, checkOverlaps);
  new G4PVPlacement(NULL, surfaceInSubworld, logicSurfaceC, "SurfaceC",
                    logicSubworldC, 0, 0, checkOverlaps);

  // Portal entrance
  //
  //

  G4double portal_sizeXY = subworld_sizeXY * griddim;
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

  G4int verboseLvL{0};
  Surface::PortalStore &portalStore = Surface::Locator::GetPortalStore();
  Surface::MultipleSubworld *portalEntrance = new Surface::MultipleSubworld(
      "Entrance", physPortal, portalPlacement, verboseLvL);

  Surface::MultipleSubworld *portalSubworldA = new Surface::MultipleSubworld(
      "Subworld-A", physSubworldA, subworldTriggerPlacementA, verboseLvL,
      fGeneratorA.GetFacetStore());
  Surface::MultipleSubworld *portalSubworldB = new Surface::MultipleSubworld(
      "Subworld-B", physSubworldB, subworldTriggerPlacementB, verboseLvL,
      fGeneratorB.GetFacetStore());
  Surface::MultipleSubworld *portalSubworldC = new Surface::MultipleSubworld(
      "Subworld-C", physSubworldC, subworldTriggerPlacementC, verboseLvL,
      fGeneratorC.GetFacetStore());

  // set trigger for portals
  portalEntrance->SetTrigger(physPortal);
  portalSubworldA->SetTrigger(physSubworldTriggerA);
  portalSubworldB->SetTrigger(physSubworldTriggerB);
  portalSubworldC->SetTrigger(physSubworldTriggerC);
  // link portals
  portalEntrance->SetAsPortal();
  portalEntrance->SetSubwordlEdge(subworld_sizeXY, subworld_sizeXY,
                                  subworld_sizeZ);
  portalEntrance->SetGrid(griddim, griddim, 0);
  portalEntrance->SetOtherPortal(portalSubworldA);
  portalSubworldA->SetOtherPortal(portalEntrance);
  portalSubworldB->SetOtherPortal(portalEntrance);
  portalSubworldC->SetOtherPortal(portalEntrance);
  Surface::HelperFillSubworldGrid<Surface::MultipleSubworld> subworldHelper(0);
  subworldHelper.AddAvailableSubworld(portalSubworldA, 0.3);
  subworldHelper.AddAvailableSubworld(portalSubworldB, 0.5);
  subworldHelper.AddAvailableSubworld(portalSubworldC, 0.2);
  subworldHelper.FillGrid(portalEntrance->GetSubworldGrid());
  portalEntrance->GetSubworldGrid()->PrintUniqueSubworlds();
  portalEntrance->GetSubworldGrid()->PrintStatistic();
  portalEntrance->GetSubworldGrid()->PrintLegend();
  portalEntrance->GetSubworldGrid()->PrintGrid(0, 10, 0, 10);

  // add them to the store
  portalStore.push_back(portalEntrance);
  portalStore.push_back(portalSubworldA);
  portalStore.push_back(portalSubworldB);
  portalStore.push_back(portalSubworldC);

  // set StepLimit

  G4double maxStepsize = 100 * mm;
  G4double maxStepsize_subworld = 0.1 * mm;
  G4UserLimits *limit = new G4UserLimits(maxStepsize);
  G4UserLimits *limit_subworld = new G4UserLimits(maxStepsize_subworld);
  logicWorld->SetUserLimits(limit);
  logicSubworldA->SetUserLimits(limit_subworld);
  logicSubworldB->SetUserLimits(limit_subworld);
  logicSubworldC->SetUserLimits(limit_subworld);
  logicPortal->SetUserLimits(limit);
  //
  // always return the physical World
  //
  return physWorld;
}
