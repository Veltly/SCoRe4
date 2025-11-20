/**
 * @brief detector construction for generating a surface
 * @file DetectorConstruction.hh
 * @date 2025-10-27
 */

#ifndef DETECTOR_CONSTRUCTION_EXAMPLE_SURFACE_PORTAL_HH
#define DETECTOR_CONSTRUCTION_EXAMPLE_SURFACE_PORTAL_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "Service/include/MultiportalHelper.hh"
#include "Service/include/RoughnessHelper.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume *Construct() override;
 void ConstructSDandField() override;
 void CheckValues() const;

protected:
 Surface::MultiportalHelper f_portal_helper;
 Surface::RoughnessHelper f_surface_helper;
private:
};

#endif //DETECTOR_CONSTRUCTION_EXAMPLE_SURFACE_PORTAL_HH
