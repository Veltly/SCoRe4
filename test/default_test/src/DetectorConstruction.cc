// Author C.Gruener
// Date 24-05-21
// File DetectorConstruction

#include "DetectorConstruction.hh"

#include <CLHEP/Geometry/Transform3D.h>

#include <G4MultiUnion.hh>
#include <G4Region.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4UserLimits.hh>
#include <cfloat>

#include "../../../src/Portal/include/MultipleSubworld.hh"
#include "../../../src/Portal/include/PortalStore.hh"
#include "../../../src/Service/include/Locator.hh"
#include "../../../src/Service/include/MultiportalHelper.hh"
#include "../../../src/Service/include/RoughnessHelper.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Portal/include/VPortal.hh"

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
  const G4double world_sizeXY = 1 * m;
  const G4double world_sizeZ = 1 * m;
  G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box *solidWorld = new G4Box("World",  // its name
                                0.5 * world_sizeXY, 0.5 * world_sizeXY,
                                0.5 * world_sizeZ);  // its size

  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                                    world_mat,   // its material
                                                    "World");    // its name

  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0,                // no rotation
                        G4ThreeVector(),  // at (0,0,0)
                        logicWorld,       // its logical volume
                        "World",          // its name
                        0,                // its mother  volume
                        false,            // no boolean operation
                        0,                // copy number
                        checkOverlaps);   // overlaps checking

  const G4ThreeVector placementA{5 * cm, 5 * cm, 22 * cm};
  const G4ThreeVector placementB{5 * cm, 5 * cm, 20 * cm};
  const G4ThreeVector placementC{5 * cm, 5 * cm, 18 * cm};

  const G4Transform3D trafoA{G4RotationMatrix(), placementA};
  const G4Transform3D trafoB{G4RotationMatrix(), placementB};
  const G4Transform3D trafoC{G4RotationMatrix(), placementC};

  const G4ThreeVector placementPortal{0 * cm, 0 * cm, 10 * cm};
  const G4Transform3D trafoPortal{G4RotationMatrix(), placementPortal};

  G4Material *subworldMaterial = nist->FindOrBuildMaterial("G4_AIR");
  G4Material *roughnessMaterial = nist->FindOrBuildMaterial("G4_Si");

  Surface::MultiportalHelper helper(5);

  const G4int subworlds_x{100};
  const G4int subworlds_y{100};

  helper.SetDxPortal(10 * cm);
  helper.SetDyPortal(10 * cm);
  helper.SetDzPortal(5. * mm);

  helper.SetDxSub(helper.GetPortalDx() / subworlds_x);
  helper.SetDySub(helper.GetPortalDy() / subworlds_y);
  helper.SetDzSub(5. * mm);

  helper.SetNxSub(subworlds_x);
  helper.SetNySub(subworlds_y);

  helper.AddSubworldPlacement(trafoA);
  helper.AddSubworldPlacement(trafoB);
  helper.AddSubworldPlacement(trafoC);

  helper.AddSubworldDensity(0.3);
  helper.AddSubworldDensity(0.5);
  helper.AddSubworldDensity(0.2);

  helper.SetMotherVolume(logicWorld);

  helper.SetPortalPlacement(trafoPortal);

  helper.SetSubworldMaterial(subworldMaterial);

  helper.SetNDifferentSubworlds(3);

  helper.SetPortalName("Portal");

  Surface::RoughnessHelper roughnessA("RoughnessA");
  Surface::RoughnessHelper roughnessB("RoughnessB");
  Surface::RoughnessHelper roughnessC("RoughnessC");

  const G4int spikes{2};

  roughnessA.NSpike_X(spikes);
  roughnessA.NSpike_Y(spikes);
  roughnessA.SpikeWidth_X(helper.GetSubworldDx() / spikes);
  roughnessA.SpikeWidth_Y(helper.GetSubworldDy() / spikes);
  roughnessA.SpikeMeanHeight(1. * mm);
  roughnessA.BasisWidth_X(helper.GetSubworldDx());
  roughnessA.BasisWidth_Y(helper.GetSubworldDy());
  roughnessA.BasisHeight(1 * mm);
  roughnessA.Spikeform(Surface::Spikeform::StandardPyramide);
  roughnessA.Material(roughnessMaterial);
  roughnessA.Generate();

  roughnessB.NSpike_X(spikes);
  roughnessB.NSpike_Y(spikes);
  roughnessB.SpikeWidth_X(helper.GetSubworldDx() / spikes);
  roughnessB.SpikeWidth_Y(helper.GetSubworldDy() / spikes);
  roughnessB.SpikeMeanHeight(2. * mm);
  roughnessB.BasisWidth_X(helper.GetSubworldDx());
  roughnessB.BasisWidth_Y(helper.GetSubworldDy());
  roughnessB.BasisHeight(1 * mm);
  roughnessB.Spikeform(Surface::Spikeform::StandardPyramide);
  roughnessB.Material(roughnessMaterial);
  roughnessB.Generate();

  roughnessC.NSpike_X(spikes);
  roughnessC.NSpike_Y(spikes);
  roughnessC.SpikeWidth_X(helper.GetSubworldDx() / spikes);
  roughnessC.SpikeWidth_Y(helper.GetSubworldDy() / spikes);
  roughnessC.SpikeMeanHeight(3. * mm);
  roughnessC.BasisWidth_X(helper.GetSubworldDx());
  roughnessC.BasisWidth_Y(helper.GetSubworldDy());
  roughnessC.BasisHeight(1 * mm);
  roughnessC.Spikeform(Surface::Spikeform::StandardPyramide);
  roughnessC.Material(roughnessMaterial);
  roughnessC.Generate();

  G4Transform3D trafo{G4RotationMatrix(),
                      G4ThreeVector(0., 0., -helper.GetSubworldDz() * 0.5)};
  helper.AddRoughness(roughnessA.LogicRoughness(), trafo,
                      roughnessA.FacetStore());
  helper.AddRoughness(roughnessB.LogicRoughness(), trafo,
                      roughnessB.FacetStore());
  helper.AddRoughness(roughnessC.LogicRoughness(), trafo,
                      roughnessC.FacetStore());

  helper.Generate();
  Surface::MultipleSubworld *subA = helper.GetSubworld(0);
  Surface::MultipleSubworld *subB = helper.GetSubworld(1);
  Surface::MultipleSubworld *subC = helper.GetSubworld(2);
  //  set StepLimit

  G4double maxStepsize = 100 * mm;
  G4double maxStepsize_subworld = 100. * mm;
  G4UserLimits *limit = new G4UserLimits(maxStepsize);
  G4UserLimits *limit_subworld = new G4UserLimits(maxStepsize_subworld);
  logicWorld->SetUserLimits(limit);
  subA->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  subB->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  subC->GetVolume()->GetLogicalVolume()->SetUserLimits(limit_subworld);
  //
  //   always return the physical World
  //
  return physWorld;
}
