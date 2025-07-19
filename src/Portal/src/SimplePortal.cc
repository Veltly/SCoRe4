/**
* @brief Simple portal is a portal between two boxlike volumes
* @file SimplePortal.hh
* @author C.Gruener
* @date 24-05-25
*/

#include "Portal/include/SimplePortal.hh"

#include "G4Navigator.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "ParticleGenerator/include/SurfaceSource.hh"

/**
 * @brief Transforms vector coordinates between the portals
 * @param vec coordinates to transform
 * @return transformed coordinates
 */
G4ThreeVector Surface::SimplePortal::TransformBetweenPortals(
    const G4ThreeVector &vec) {
  const G4VPhysicalVolume *volume = GetVolume();
  const G4VPhysicalVolume *otherVolume = fOtherPortal->GetVolume();

  G4ThreeVector pMin, pMax;
  volume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  const G4ThreeVector volumeDistance = pMax - pMin;

  otherVolume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  const G4ThreeVector otherVolumeDistance = pMax - pMin;

  G4ThreeVector transformedVec = vec;
  transformedVec.setX(vec.x() / volumeDistance.x() * otherVolumeDistance.x());
  transformedVec.setY(vec.y() / volumeDistance.y() * otherVolumeDistance.y());
  transformedVec.setZ(vec.z() / volumeDistance.z() * otherVolumeDistance.z());

  return transformedVec;
}

void Surface::SimplePortal::SetOtherPortal(Surface::SimplePortal *otherPortal) {
  fOtherPortal = otherPortal;
}

/**
 * @brief Performs the portation between the entrance and exit portal
 * @param step
 */
void Surface::SimplePortal::DoPortation(G4Step *step) {
  G4ThreeVector prePosition = step->GetPreStepPoint()->GetPosition();

  fLogger.WriteDebugInfo("PrePosition: ", prePosition);
  TransformToLocalCoordinate(prePosition);

  // Transformation of position
  TransformBetweenPortals(prePosition);
  fOtherPortal->TransformToGlobalCoordinate(prePosition);
  fLogger.WriteDebugInfo("PostPosition: ", prePosition);

  UpdatePosition(step, prePosition);
}
