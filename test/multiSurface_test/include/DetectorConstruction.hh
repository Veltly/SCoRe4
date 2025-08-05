/**
 * @brief example of combination of modules portal and surface
 * @file DetectorConstruction.hh
 * @date 2024-05-21
 */

#ifndef DetectorConstruction_MultiSurface_hh
#define DetectorConstruction_MultiSurface_hh

#include "../../../src/SurfaceGenerator/include/Generator.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume *Construct() override;

protected:
  Surface::SurfaceGenerator fGeneratorA{"GenA"};
  Surface::SurfaceGenerator fGeneratorB{"GenB"};
  Surface::SurfaceGenerator fGeneratorC{"GenC"};
  G4LogicalVolume *fScoringVolume;
};

#endif
