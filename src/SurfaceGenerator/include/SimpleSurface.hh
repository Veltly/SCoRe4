// Copyright [2024] C.Gruener
// Date: 24-05-24
// File: Surface.hh

#ifndef SRC_SURFACEGENERATOR_INCLUDE_SIMPLESURFACE_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_SIMPLESURFACE_HH_

#include <G4LogicalVolume.hh>
namespace Surface {

class SimpleSurface {
 public:
  SimpleSurface();
  G4LogicalVolume *GetRoughSurface();
  // G4LogicalVolume *GetSurface();
  void SetSizeX(G4double sizeX);
  void SetSizeY(G4double sizeY);

 private:
  G4LogicalVolume *fRoughSurface;
  G4double sizeX;
  G4double sizeY;
  G4double spikeNrX;
  G4double spikeNrY;
  G4double basisSizeZ;
  G4double meanHeight;
  G4Material *subworldMaterial;
  G4Material *roughnessMaterial;
};
}  // namespace Surface
#endif  // SRC_SURFACEGENERATOR_INCLUDE_SIMPLESURFACE_HH_
