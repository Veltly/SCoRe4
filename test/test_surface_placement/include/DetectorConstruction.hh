/**
 * @brief detector construction for generating a surface
 * @file DetectorConstruction.hh
 * @date 2024-05-21
 */

#ifndef DetectorConstruction_Surface_h
#define DetectorConstruction_Surface_h

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
  G4LogicalVolume *fScoringVolume;

private:
};

#endif
