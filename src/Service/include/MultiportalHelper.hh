// Author: C.Gruener
// Date: 24-07-10
// File: MultiportalHelper
//

#ifndef SURFACE_MULTIPORTALHELPER
#define SURFACE_MULTIPORTALHELPER

#include "../../Portal/include/MultipleSubworld.hh"
#include <G4LogicalVolume.hh>

namespace Surface {
class MultiportalHelper {

public:
  MultiportalHelper(const G4int verboseLvl = 3);

  void Generate();

  // Setter
  void SetDxPortal(const G4double val);
  void SetDyPortal(const G4double val);
  void SetDzPortal(const G4double val);

  void SetDxSub(const G4double val);
  void SetDySub(const G4double val);
  void SetDzSub(const G4double val);

  void AddSubworldPlacement(const G4Transform3D &trafo);
  void AddSubworldDensity(const G4double density);

  void SetPortalPlacement(const G4Transform3D &trafo);

  void SetMotherVolume(G4LogicalVolume *motherVolume);

  void SetVerbose(const G4int verboseLvl);

  void SetSubworldMaterial(G4Material *mat);

  void SetNxSub(const G4int val);
  void SetNySub(const G4int val);

  void SetNDifferentSubworlds(const G4int val);

  // Getter
  Surface::MultipleSubworld *GetSubworld(const G4int nr);

  // Verbose
  // TODO: add verbose functions
  void PrintInfo();

private:
  void CheckValues();
  void GenerateSubworlds();
  void GeneratePortal();
  void LinkPortalWithSubworlds();
  void FillSubworldmap();

private:
  // General Infos
  G4int fNOfDifferentSubworlds;
  G4bool fCheckOverlaps;
  G4LogicalVolume *fMotherVolume;
  G4Transform3D fPlacementPortal;
  G4int fVerbose;

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

  Surface::Logger fLogger;
};
} // namespace Surface
#endif // SURFACE_MULTIPORTALHELPER
