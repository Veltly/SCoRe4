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
#include "../../../src/Service/include/MultiportalHelper.hh"
#include "../../../src/Service/include/RoughnessHelper.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fPortalHelper("PortalHelper"),
      fRoughnessHelperA("RoughnessHelperA"),
      fRoughnessHelperB("RoughnessHelperB"),
      fRoughnessHelperC("RoughnessHelperC"),
      fScoringVolume(0) {}

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

  // G4Material *subworldMaterial = nist->FindOrBuildMaterial("G4_AIR");
  // G4Material *roughnessMaterial = nist->FindOrBuildMaterial("G4_Si");
  //
  // const G4int subworlds_x{2};
  // const G4int subworlds_y{2};

  // fPortalHelper.SetDxPortal(10 * cm);
  // fPortalHelper.SetDyPortal(10 * cm);
  // fPortalHelper.SetDzPortal(5. * mm);
  //
  // fPortalHelper.SetDxSub(fPortalHelper.GetPortalDx() / subworlds_x);
  // fPortalHelper.SetDySub(fPortalHelper.GetPortalDy() / subworlds_y);
  // fPortalHelper.SetDzSub(5. * mm);
  //
  // fPortalHelper.SetNxSub(subworlds_x);
  // fPortalHelper.SetNySub(subworlds_y);

  fPortalHelper.AddSubworldPlacement(trafoA);
  fPortalHelper.AddSubworldPlacement(trafoB);
  fPortalHelper.AddSubworldPlacement(trafoC);

  fPortalHelper.AddSubworldDensity(0.3);
  fPortalHelper.AddSubworldDensity(0.5);
  fPortalHelper.AddSubworldDensity(0.2);

  fPortalHelper.SetMotherVolume(logicWorld);

  fPortalHelper.SetPortalPlacement(trafoPortal);

  // fPortalHelper.SetSubworldMaterial(subworldMaterial);

  fPortalHelper.SetNDifferentSubworlds(3);

  fPortalHelper.SetPortalName("Portal");

  // const G4int spikes{2};

  // fRoughnessHelperA.SetSpikeNx(spikes);
  // fRoughnessHelperA.SetSpikeNy(spikes);
  // fRoughnessHelperA.SetSpikeDx(fPortalHelper.GetSubworldDx() / spikes);
  // fRoughnessHelperA.SetSpikeDy(fPortalHelper.GetSubworldDy() / spikes);
  // fRoughnessHelperA.SetSpikeMeanHeight(1. * mm);
  // fRoughnessHelperA.SetBasisDx(fPortalHelper.GetSubworldDx());
  // fRoughnessHelperA.SetBasisDy(fPortalHelper.GetSubworldDy());
  // fRoughnessHelperA.SetBasisHeight(1 * mm);
  // fRoughnessHelperA.SetSpikeform(Surface::Spikeform::StandardPyramide);
  // fRoughnessHelperA.SetMaterial(roughnessMaterial);
  fRoughnessHelperA.Generate();

  // fRoughnessHelperB.SetSpikeNx(spikes);
  // fRoughnessHelperB.SetSpikeNy(spikes);
  // fRoughnessHelperB.SetSpikeDx(fPortalHelper.GetSubworldDx() / spikes);
  // fRoughnessHelperB.SetSpikeDy(fPortalHelper.GetSubworldDy() / spikes);
  // fRoughnessHelperB.SetSpikeMeanHeight(2. * mm);
  // fRoughnessHelperB.SetBasisDx(fPortalHelper.GetSubworldDx());
  // fRoughnessHelperB.SetBasisDy(fPortalHelper.GetSubworldDy());
  // fRoughnessHelperB.SetBasisHeight(1 * mm);
  // fRoughnessHelperB.SetSpikeform(Surface::Spikeform::StandardPyramide);
  // fRoughnessHelperB.SetMaterial(roughnessMaterial);
  fRoughnessHelperB.Generate();

  // fRoughnessHelperC.SetSpikeNx(spikes);
  // fRoughnessHelperC.SetSpikeNy(spikes);
  // fRoughnessHelperC.SetSpikeDx(fPortalHelper.GetSubworldDx() / spikes);
  // fRoughnessHelperC.SetSpikeDy(fPortalHelper.GetSubworldDy() / spikes);
  // fRoughnessHelperC.SetSpikeMeanHeight(3. * mm);
  // fRoughnessHelperC.SetBasisDx(fPortalHelper.GetSubworldDx());
  // fRoughnessHelperC.SetBasisDy(fPortalHelper.GetSubworldDy());
  // fRoughnessHelperC.SetBasisHeight(1 * mm);
  // fRoughnessHelperC.SetSpikeform(Surface::Spikeform::StandardPyramide);
  // fRoughnessHelperC.SetMaterial(roughnessMaterial);
  fRoughnessHelperC.Generate();

  G4Transform3D trafo{
      G4RotationMatrix(),
      G4ThreeVector(0., 0., -fPortalHelper.GetSubworldDz() + 2. * CLHEP::mm)};

  fPortalHelper.AddRoughness(fRoughnessHelperA.LogicRoughness(), trafo,
                             fRoughnessHelperA.FacetStore());
  fPortalHelper.AddRoughness(fRoughnessHelperB.LogicRoughness(), trafo,
                             fRoughnessHelperB.FacetStore());
  fPortalHelper.AddRoughness(fRoughnessHelperC.LogicRoughness(), trafo,
                             fRoughnessHelperC.FacetStore());

  fPortalHelper.Generate();
  Surface::MultipleSubworld *subA = fPortalHelper.GetSubworld(0);
  Surface::MultipleSubworld *subB = fPortalHelper.GetSubworld(1);
  Surface::MultipleSubworld *subC = fPortalHelper.GetSubworld(2);
  //  set StepLimit

  G4double maxStepsize = 1 * CLHEP::mm;
  G4double maxStepsize_subworld = 100. * CLHEP::nm;
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
