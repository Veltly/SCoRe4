/**
 * @brief detector construction for generating a surface
 * @file DetectorConstruction.hh
 * @date 2025-10-27
 */

#ifndef DETECTOR_CONSTRUCTION_EXAMPLE_SURFACE_HH
#define DETECTOR_CONSTRUCTION_EXAMPLE_SURFACE_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume *Construct() override;
 void ConstructSDandField() override;

protected:
  G4LogicalVolume *fScoringVolume;

private:
};

#endif //DETECTOR_CONSTRUCTION_EXAMPLE_SURFACE_HH
