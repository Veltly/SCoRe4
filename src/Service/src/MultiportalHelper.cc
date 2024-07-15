// Copyright [2024] C.Gruener
// Date: 24-07-10
// File: MultiportalHelper
//

#include "../include/MultiportalHelper.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/PortalStore.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Service/include/Locator.hh"

#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Transform3D.hh>
#include <G4VPhysicalVolume.hh>

Surface::MultiportalHelper::MultiportalHelper(const G4int verboseLvl)
    : fLogger({"MultiPortalHelper", verboseLvl}) {}

void Surface::MultiportalHelper::CheckValues() {
  fLogger.WriteInfo("All values correct");
}

void Surface::MultiportalHelper::GenerateSubworlds() {
  Surface::PortalStore &portalStore = Surface::Locator::GetPortalStore();

  for (G4int i = 0; i < fNOfDifferentSubworlds; ++i) {
    const G4Transform3D transformation = fPlacementSub.at(i);

    // Create Trigger
    const G4String nameTrig = fSubName + "_Trigger_" + std::to_string(i);
    G4Box *solidTrig =
        new G4Box(nameTrig, 1.1 * fDxSub, 1.1 * fDySub, 1.1 * fDzSub);
    G4LogicalVolume *logicTrig =
        new G4LogicalVolume(solidTrig, fSubworldMaterial, nameTrig);
    G4VPhysicalVolume *physTrig =
        new G4PVPlacement(transformation, logicTrig, nameTrig, fMotherVolume,
                          false, 0, fCheckOverlaps);

    // Create Subworld
    const G4String nameSub = fSubName + "Subworld_" + std::to_string(i);
    G4Box *solidSub = new G4Box(nameSub, fDxSub, fDySub, fDzSub);
    G4LogicalVolume *logicSub =
        new G4LogicalVolume(solidSub, fSubworldMaterial, nameSub);
    const G4ThreeVector placementSub{0., 0., 0.};
    G4VPhysicalVolume *physSub =
        new G4PVPlacement(0, placementSub, logicSub, nameSub, logicTrig, false,
                          0, fCheckOverlaps);

    // Create Subworld
    Surface::MultipleSubworld *portalSubworld = new Surface::MultipleSubworld(
        nameSub, physSub, transformation, fVerbose);

    // set trigger
    portalSubworld->SetTrigger(physTrig);

    // add to portal store
    portalStore.push_back(portalSubworld);

    // add to internal store
    fMultipleSubworld.push_back(portalSubworld);
  }
  fLogger.WriteInfo("Generated " + std::to_string(fNOfDifferentSubworlds) +
                    " subworlds and trigger");
}

void Surface::MultiportalHelper::GeneratePortal() {
  const G4String namePortal = fPortalName;

  G4Box *solidPortal = new G4Box(namePortal, fDxSub, fDySub, fDzSub);
  G4LogicalVolume *logicPortal =
      new G4LogicalVolume(solidPortal, fSubworldMaterial, namePortal);
  G4VPhysicalVolume *physPortal =
      new G4PVPlacement(fPlacementPortal, logicPortal, namePortal,
                        fMotherVolume, false, 0, fCheckOverlaps);

  fPortal = new Surface::MultipleSubworld(namePortal, physPortal,
                                          fPlacementPortal, fVerbose);

  fPortal->SetTrigger(physPortal);
  fPortal->SetAsPortal();
  fPortal->SetSubworldEdge(2 * fDxSub, 2 * fDySub, 2 * fDzSub);
  fPortal->SetGrid(fNx, fNy);

  Surface::PortalStore &portalStore = Surface::Locator::GetPortalStore();

  portalStore.push_back(fPortal);
  fLogger.WriteInfo("Generated Portal " + namePortal);
}

void Surface::MultiportalHelper::LinkPortalWithSubworlds() {
  fPortal->SetOtherPortal(fMultipleSubworld.at(0));
  for (auto &subworld : fMultipleSubworld) {
    subworld->SetOtherPortal(fPortal);
  }
  fLogger.WriteInfo("Linked Portal with Subworlds");
}

void Surface::MultiportalHelper::FillSubworldmap() {
  Surface::HelperFillSubworldGrid<Surface::MultipleSubworld> mapHelper(
      fVerbose);
  for (G4int i = 0; i < fNOfDifferentSubworlds; ++i) {
    Surface::MultipleSubworld *subworld = fMultipleSubworld.at(i);
    const G4double density = fSubworldProb.at(i);
    mapHelper.AddAvailableSubworld(subworld, density);
  }

  mapHelper.FillGrid(fPortal->GetSubworldGrid());
  fLogger.WriteInfo("Filled map Portal<->Subworlds");
}

void Surface::MultiportalHelper::Generate() {
  // Do a check of all sizes and information, if needed calculate missing
  // parts
  CheckValues();
  // Generate Subworld, Trigger, combine it, place it
  GenerateSubworlds();
  // Generate Portal, place it, link Portal and Subworlds
  GeneratePortal();
  // link all
  LinkPortalWithSubworlds();
  // Create Helper, fill and use it
  FillSubworldmap();

  fLogger.WriteInfo("Generated Portal with Subworlds");

  if (fLogger.WriteDetailInfo()) {
    PrintInfo();
  }
}

// Setter

void Surface::MultiportalHelper::SetDxPortal(const G4double val) { fDx = val; }
void Surface::MultiportalHelper::SetDyPortal(const G4double val) { fDy = val; }
void Surface::MultiportalHelper::SetDzPortal(const G4double val) { fDz = val; }

void Surface::MultiportalHelper::SetDxSub(const G4double val) { fDxSub = val; }
void Surface::MultiportalHelper::SetDySub(const G4double val) { fDySub = val; }
void Surface::MultiportalHelper::SetDzSub(const G4double val) { fDzSub = val; }

void Surface::MultiportalHelper::AddSubworldPlacement(
    const G4Transform3D &trafo) {
  fPlacementSub.push_back(trafo);
}
void Surface::MultiportalHelper::AddSubworldDensity(const G4double density) {
  fSubworldProb.push_back(density);
}

void Surface::MultiportalHelper::SetPortalPlacement(
    const G4Transform3D &trafo) {
  fPlacementPortal = trafo;
}

void Surface::MultiportalHelper::SetMotherVolume(
    G4LogicalVolume *motherVolume) {
  fMotherVolume = motherVolume;
}

void Surface::MultiportalHelper::SetVerbose(const G4int verboseLvl) {
  fVerbose = verboseLvl;
}

void Surface::MultiportalHelper::SetSubworldMaterial(G4Material *mat) {
  fSubworldMaterial = mat;
}

void Surface::MultiportalHelper::SetNxSub(const G4int val) { fNx = val; }
void Surface::MultiportalHelper::SetNySub(const G4int val) { fNy = val; }

void Surface::MultiportalHelper::SetNDifferentSubworlds(const G4int val) {
  fNOfDifferentSubworlds = val;
}

Surface::MultipleSubworld *
Surface::MultiportalHelper::GetSubworld(const G4int id) {
  return fMultipleSubworld.at(id);
}

void Surface::MultiportalHelper::SetPortalName(const G4String name) {
  fPortalName = name;
}

void Surface::MultiportalHelper::SetSubworldName(const G4String name) {
  fSubName = name;
}

std::stringstream Surface::MultiportalHelper::StreamInfo() const {
  auto trafoString = [](const G4Transform3D &trafo) {
    const G4ThreeVector vec = trafo.getTranslation();
    std::stringstream ss;
    ss << "x: " << vec.x() << " y: " << vec.y() << " z: " << vec.z();
    return ss.str();
  };

  std::stringstream ss;
  ss << "\n"
     << "************************************\n"
     << "**** State of MultiportalHelper ****\n"
     << "************************************\n"
     << "\n"
     << "Info Portal: " << fPortalName << "\n"
     << "Dx: " << fDx << "\n"
     << "Dy: " << fDy << "\n"
     << "Dz: " << fDz << "\n"
     << "Placement Portal: " << trafoString(fPlacementPortal) << "\n"
     << "\n\n";

  ss << "Info Subworlds:\n"
     << "Dx: " << fDxSub << "\n"
     << "Dy: " << fDySub << "\n"
     << "Dz: " << fDzSub << "\n"
     << "\n"
     << "Name          Placement          Density\n";
  for (G4int i = 0; i < fNOfDifferentSubworlds; ++i) {
    ss << "Subworld_" << i << ": " << trafoString(fPlacementSub.at(i)) << " , "
       << fSubworldProb.at(i) << "\n";
  }
  ss << "\n"
     << fPortal->GetSubworldGrid()->StreamStatistic().str() << "\n"
     << "Material: " << fSubworldMaterial->GetName() << "\n"
     << "\n\n";

  ss << "Grid:\n"
     << "Nx: " << fNx << "\n"
     << "Ny: " << fNy << "\n"
     << "Sum: " << fNx * fNy << "\n"
     << "\n";
  ss << "************************************\n"
     << "************************************\n";
  return ss;
}

void Surface::MultiportalHelper::PrintInfo() const {
  G4cout << StreamInfo().str() << G4endl;
}

// Getter

G4double Surface::MultiportalHelper::GetPortalDx() const { return fDx; }
G4double Surface::MultiportalHelper::GetPortalDy() const { return fDy; }
G4double Surface::MultiportalHelper::GetPortalDz() const { return fDz; }
G4double Surface::MultiportalHelper::GetSubworldDx() const { return fDxSub; }
G4double Surface::MultiportalHelper::GetSubworldDy() const { return fDySub; }
G4double Surface::MultiportalHelper::GetSubworldDz() const { return fDzSub; }
G4Material *Surface::MultiportalHelper::GetSubworldMaterial() const {
  return fSubworldMaterial;
}

G4Transform3D Surface::MultiportalHelper::GetSubworldPlacement(const G4int id) {
  return fPlacementSub.at(id);
}
