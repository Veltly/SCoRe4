// Copyright [2024] C.Gruener
// Author: C.Gruener
// Date: 24-07-10
// File: MultiportalHelper
//

#ifndef SRC_SERVICE_INCLUDE_MULTIPORTALHELPER_HH_
#define SRC_SERVICE_INCLUDE_MULTIPORTALHELPER_HH_

#include <vector>

#include "G4LogicalVolume.hh"
#include "Portal/include/MultipleSubworld.hh"

namespace Surface {
class MultiportalHelperMessenger;

class MultiportalHelper {
 public:
  explicit MultiportalHelper(const G4String &helperName);
  MultiportalHelper(const G4String &helperName, G4int verboseLvl);

  void Generate();

  // Setter
  void SetDxPortal(G4double val);
  void SetDyPortal(G4double val);
  void SetDzPortal(G4double val);

  void SetDxSub(G4double val);
  void SetDySub(G4double val);
  void SetDzSub(G4double val);

  void AddSubworldPlacement(const G4Transform3D &trafo);
  void AddSubworldDensity(G4double density);

  void SetPortalPlacement(const G4Transform3D &trafo);

  void SetMotherVolume(G4LogicalVolume *motherVolume);

  void SetVerbose(G4int verboseLvl);

  void SetSubworldMaterial(G4Material *mat);
  void SetSubworldMaterial(const G4String &materialName);

  void SetNxSub(G4int val);
  void SetNySub(G4int val);

  void SetNDifferentSubworlds(G4int val);

  void SetPortalName(const G4String &name);
  void SetSubworldName(const G4String &name);

  void AddRoughness(G4LogicalVolume *, const G4Transform3D &, FacetStore *);

  // Getter
  G4String GetName() const { return fHelperName; }

  Surface::MultipleSubworld *GetSubworld(G4int id) const;

  G4double GetPortalDx() const;
  G4double GetPortalDy() const;
  G4double GetPortalDz() const;

  G4double GetSubworldDx() const;
  G4double GetSubworldDy() const;
  G4double GetSubworldDz() const;

  G4Material *GetSubworldMaterial() const;
  G4Transform3D GetSubworldPlacement(G4int id) const;
  // Verbose
  void PrintInfo() const;
  std::stringstream StreamInfo() const;

 private:
  void CheckValues() const;
  void GenerateSubworlds();
  void GeneratePortal();
  void LinkPortalWithSubworlds();
  void FillSubworldMap() const;
  void AddRoughness();

 private:
  // General Infos
  const G4String fHelperName;
  G4int fNOfDifferentSubworlds;
  G4bool fCheckOverlaps;
  G4LogicalVolume *fMotherVolume;
  G4Transform3D fPlacementPortal;
  G4int fVerbose;
  Surface::Logger fLogger;
  MultiportalHelperMessenger *fMessenger;

  // Subworld
  // Dimension
  G4double fDxSub;
  G4double fDySub;
  G4double fDzSub;

  G4Material *fSubworldMaterial;

  std::vector<G4Transform3D> fPlacementSub;
  std::vector<G4double> fSubworldProb;
  std::vector<Surface::MultipleSubworld *> fMultipleSubworld;

  // Portal
  // Dimension
  G4double fDx;
  G4double fDy;
  G4double fDz;
  // Number of Subworlds
  G4int fNx;
  G4int fNy;

  Surface::MultipleSubworld *fPortal;

  G4String fPortalName;
  G4String fSubName;

  // Roughness (optional)
  std::vector<G4LogicalVolume *> fRoughness;
  std::vector<FacetStore *> fFacetStore;
  std::vector<G4Transform3D> fTransformRoughness;
};
}  // namespace Surface
#endif  // SRC_SERVICE_INCLUDE_MULTIPORTALHELPER_HH_
