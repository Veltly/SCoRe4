/**
 * @brief Implementation of helper class MultiportalHelper
 * @author C.Gruener
 * @date 2024-07-10
 * @file MultiportalHelper.cc
 */

#include "Service/include/MultiportalHelper.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4Transform3D.hh"
#include "G4VPhysicalVolume.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/PortalStore.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Service/include/Locator.hh"
#include "Service/include/MultiportalHelperMessenger.hh"

Surface::MultiportalHelper::MultiportalHelper(const G4String &helperName,
                                              const VerboseLevel verboseLvl)
    : fHelperName(helperName),
      fNOfDifferentSubworlds(0),
      fCheckOverlaps(false),
      fMotherVolume(nullptr),
      fLogger("MPH_" + helperName, verboseLvl),
      fMessenger(new MultiportalHelperMessenger(this, helperName)),
      fDxSub(0),
      fDySub(0),
      fDzSub(0),
      fSubworldMaterial(nullptr),
      fDx(0),
      fDy(0),
      fDz(0),
      fNx(0),
      fNy(0),
      fPortal(nullptr) {}

void Surface::MultiportalHelper::CheckValues() const {
  auto isSame = [](const G4double valA, const G4double valB) {
    constexpr G4double numericLimit = std::numeric_limits<G4double>::epsilon() * 10;
    return std::fabs(valA - valB) < numericLimit;
  };

  G4bool error{false};
  std::stringstream ss;
  // X-dimension
  const G4double allSubworldX = fNx * fDxSub;
  if (!isSame(allSubworldX, fDx)) {
    ss << "Subworld does not fit in portal (X-Dimension)!\n";
    ss << "SubworldDx * NrSubworlds != PortalDx\n";
    ss << fDxSub / CLHEP::mm << " mm * " << fNx << " != " << fDx / CLHEP::mm
       << " mm\n";
    ss << "\n";
    error = true;
  }

  const G4double allSubworldY = fNy * fDySub;
  if (!isSame(allSubworldY, fDy)) {
    ss << "Subworld does not fit in portal (Y-Dimension)!\n";
    ss << "SubworldDy * NrSubworlds != PortalDy\n";
    ss << fDySub / CLHEP::mm << " mm * " << fNy << " != " << fDy / CLHEP::mm
       << " mm\n";
    ss << "\n";
    error = true;
  }

  if (!isSame(fDzSub, fDz)) {
    ss << "Subworld does not fit in portal (Z-Dimension)!\n";
    ss << "SubworldDz != PortalDz\n";
    ss << fDzSub / CLHEP::mm << " mm != " << fDz / CLHEP::mm << " mm\n";
    ss << "\n";
    error = true;
  }

  if (error) {
    std::stringstream stream;
    stream << "\n";
    stream << "**************************************************\n";
    stream << "*                                                *\n";
    stream << "* Error while generation of " << std::setw(21) << std::left
           << fHelperName << "*\n";
    stream << "* Portal: " << std::setw(39) << std::left << fPortalName
           << "*\n";
    stream << "* Subworld: " << std::setw(37) << std::left << fSubName << "*\n";
    stream << "*                                                *\n";
    stream << "**************************************************\n";
    stream << "\n";
    ss << "**************************************************\n";
    ss << "**************************************************\n";
    ss << "\n";

    fLogger.WriteError(stream.str() + ss.str());
    exit(EXIT_FAILURE);
  }
  fLogger.WriteInfo("Values correct");
}

void Surface::MultiportalHelper::GenerateSubworlds() {
  Surface::PortalStore &portalStore = Surface::Locator::GetPortalStore();

  for (G4int i = 0; i < fNOfDifferentSubworlds; ++i) {
    const G4Transform3D transformation = fPlacementSub.at(i);

    // Create Trigger
    const G4String nameTrig = fSubName + "_Trigger_" + std::to_string(i);
    auto solidTrig =
        new G4Box(nameTrig, 1.1 * fDxSub, 1.1 * fDySub, 1.1 * fDzSub);
    auto logicTrig =
        new G4LogicalVolume(solidTrig, fSubworldMaterial, nameTrig);
    G4VPhysicalVolume *physTrig =
        new G4PVPlacement(transformation, logicTrig, nameTrig, fMotherVolume,
                          false, 0, fCheckOverlaps);

    // Create Subworld
    const G4String nameSub = fSubName + "Subworld_" + std::to_string(i);
    auto solidSub = new G4Box(nameSub, fDxSub, fDySub, fDzSub);
    auto logicSub =
        new G4LogicalVolume(solidSub, fSubworldMaterial, nameSub);
    const G4ThreeVector placementSub{0., 0., 0.};
    G4VPhysicalVolume *physSub =
        new G4PVPlacement(nullptr, placementSub, logicSub, nameSub, logicTrig, false,
                          0, fCheckOverlaps);

    // Create Subworld
    auto *portalSubworld = new Surface::MultipleSubworld(
        nameSub, physSub, transformation, fLogger.GetVerboseLvl());

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

  auto solidPortal = new G4Box(namePortal, fDx, fDy, fDz);
  auto *logicPortal =
      new G4LogicalVolume(solidPortal, fSubworldMaterial, namePortal);
  G4VPhysicalVolume *physPortal =
      new G4PVPlacement(fPlacementPortal, logicPortal, namePortal,
                        fMotherVolume, false, 0, fCheckOverlaps);

  fPortal = new Surface::MultipleSubworld(namePortal, physPortal,
                                          fPlacementPortal, fLogger.GetVerboseLvl());

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

void Surface::MultiportalHelper::FillSubworldMap() const {
  Surface::HelperFillSubworldGrid<Surface::MultipleSubworld> mapHelper(
      fLogger.GetVerboseLvl());
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
  FillSubworldMap();

  AddRoughness();
  fLogger.WriteInfo("Generated Portal with Subworlds");

  fLogger.WriteDetailInfo([this] {return InfoString();});
  //if (fLogger.IsDetailInfoLvl()) {
  //  InfoString();
  //}
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

void Surface::MultiportalHelper::SetSubworldMaterial(G4Material *mat) {
  fSubworldMaterial = mat;
}

void Surface::MultiportalHelper::SetSubworldMaterial(
    const G4String &materialName) {
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *material = nist->FindOrBuildMaterial(materialName);
  SetSubworldMaterial(material);
}

void Surface::MultiportalHelper::SetNxSub(const G4int val) { fNx = val; }
void Surface::MultiportalHelper::SetNySub(const G4int val) { fNy = val; }

void Surface::MultiportalHelper::SetNDifferentSubworlds(const G4int val) {
  fNOfDifferentSubworlds = val;
}

Surface::MultipleSubworld *Surface::MultiportalHelper::GetSubworld(
    const G4int id) const {
  return fMultipleSubworld.at(id);
}

void Surface::MultiportalHelper::SetPortalName(const G4String &name) {
  fPortalName = name;
}

void Surface::MultiportalHelper::SetSubworldName(const G4String &name) {
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
  ss << "\n";
  ss << "\n";
  ss << "************************************\n";
  ss << "**** State of MultiportalHelper ****\n";
  ss << "************************************\n";
  ss << "\n";
  ss << "Info Portal: " << fPortalName << "\n";
  ss << "Dx: " << fDx << "\n";
  ss << "Dy: " << fDy << "\n";
  ss << "Dz: " << fDz << "\n";
  ss << "Placement Portal: " << trafoString(fPlacementPortal) << "\n";
  ss << "\n\n";

  ss << "Info Subworlds:\n";
  ss << "Dx: " << fDxSub << "\n";
  ss << "Dy: " << fDySub << "\n";
  ss << "Dz: " << fDzSub << "\n";
  ss << "\n";
  ss << "Name          Placement          Density\n";
  for (G4int i = 0; i < fNOfDifferentSubworlds; ++i) {
    ss << "Subworld_" << i << ": " << trafoString(fPlacementSub.at(i)) << " , "
       << fSubworldProb.at(i) << "\n";
  }
  ss << "\n";
  ss << fPortal->GetSubworldGrid()->StreamStatistic().str() << "\n";
  ss << "Material: " << fSubworldMaterial->GetName() << "\n";
  ss << "\n";
  ss << "\n";

  ss << "Grid:\n";
  ss << "Nx: " << fNx << "\n";
  ss << "Ny: " << fNy << "\n";
  ss << "Sum: " << fNx * fNy << "\n";
  ss << "\n";
  ss << "************************************\n";
  ss << "************************************\n";
  return ss;
}

std::string Surface::MultiportalHelper::InfoString() const {
  return StreamInfo().str();
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

G4Transform3D Surface::MultiportalHelper::GetSubworldPlacement(const G4int id) const {
  return fPlacementSub.at(id);
}

void Surface::MultiportalHelper::AddRoughness() {
  if (fRoughness.empty()) {
    return;
  }
  for (size_t id = 0; id < fRoughness.size(); ++id) {
    const G4String name = fSubName + "_roughness" + std::to_string(id);
    G4LogicalVolume *roughness = fRoughness.at(id);
    G4LogicalVolume *motherVolume =
        fMultipleSubworld.at(id)->GetVolume()->GetLogicalVolume();
    const G4Transform3D trafo = fTransformRoughness.at(id);

    new G4PVPlacement(trafo, roughness, name, motherVolume, false, 0,
                      fCheckOverlaps);
    // Set FacetStore
    fMultipleSubworld.at(id)->SetFacetStore(fFacetStore.at(id));

    const G4ThreeVector translation =
        fMultipleSubworld.at(id)->GetVolume()->GetTranslation() +
        trafo.getTranslation();
    const G4Transform3D trafoRoughness = fPlacementSub.at(id) * trafo;
    fFacetStore.at(id)->SetTransformation(trafoRoughness);

    G4cout << "TestTest Roughness "
           << fMultipleSubworld.at(id)->GetVolume()->GetTranslation() << G4endl;
    G4cout << "TestTest Roughness " << trafo.getTranslation() << G4endl;
  }
}

void Surface::MultiportalHelper::AddRoughness(G4LogicalVolume *vol,
                                              const G4Transform3D &trafo,
                                              FacetStore *facetStore) {
  fRoughness.push_back(vol);
  fTransformRoughness.push_back(trafo);
  fFacetStore.push_back(facetStore);
}

void Surface::MultiportalHelper::SetVerbose(const G4int verboseLvl) {
  fLogger.SetVerboseLvl(verboseLvl);
}

void Surface::MultiportalHelper::SetVerbose(const Surface::VerboseLevel verboseLvl) {
  fLogger.SetVerboseLvl(verboseLvl);
}