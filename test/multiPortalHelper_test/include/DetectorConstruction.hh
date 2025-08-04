/**
 * @brief Definition of the DetectorConstruction class
 * @file DetectorConstruction.hh
 */

#ifndef DetectorConstruction_MultiportalHelper_h
#define DetectorConstruction_MultiportalHelper_h

#include "../../../src/SurfaceGenerator/include/Generator.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.
class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume *Construct() override;

protected:
  G4LogicalVolume *fScoringVolume;
};

#endif
