/**
 * @brief detector setup for test of surface shift
 * @file DetectorConstruction.hh
 * @author C.Gruener
 * @date 2024-05-21
 */

#ifndef DetectorConstruction_surfaceShift_hh
#define DetectorConstruction_surfaceShift_hh

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume *Construct() override;

protected:
  G4LogicalVolume *fScoringVolume;
};

#endif
