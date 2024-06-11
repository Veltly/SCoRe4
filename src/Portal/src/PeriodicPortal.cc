// Author: C.Gruener
// Date: 24-06-04
// File: PeriodicPortal

#include "../include/PeriodicPortal.hh"
#include "../include/VPortal.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include <cstdlib>
#include <string>

Surface::PeriodicPortal::PeriodicPortal(G4String name,
                                        G4VPhysicalVolume *volume,
                                        G4ThreeVector &vec, G4int verbose)
    : VPortal(name, volume, PortalType::PeriodicPortal),
      fLogger({"PeriodicPortal", verbose}), fIsPortal(false) {
  SetGlobalCoord(vec);
};

void Surface::PeriodicPortal::DoPortation(const G4Step *step) {
  // select portation method by checking Grid and side of exit portal
  SingleSurface surface = GetNearestSurface(step);
  PortationType portationType = GetPortationType(surface);
  switch (portationType) {
  case PortationType::ENTER: {
    fLogger.WriteDebugInfo("Doing portation of type: Enter");
    EnterPortal(step, surface);
    break;
  }
  case PortationType::EXIT: {
    fLogger.WriteDebugInfo("Doing portation of type: Exit");
    ExitPortal(step, surface);
    break;
  }
  case PortationType::PERIODIC: {
    fLogger.WriteDebugInfo("Doing portation of type: Periodic");
    DoPeriodicPortation(step, surface);
    break;
  }
  }
};

void Surface::PeriodicPortal::EnterPortal(const G4Step *step,
                                          SingleSurface enterSurface) {

  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  position = TransformToLocalCoordinate(position);
  TransformPortalToSubworld(position);
  position = fOtherPortal->TransformToGlobalCoordinate(position);
  UpdatePosition(step, position);
}

void Surface::PeriodicPortal::ExitPortal(const G4Step *step,
                                         SingleSurface exitSurface) {
  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  position = TransformToLocalCoordinate(position);
  TransformSubworldToPortal(position);
  position = fOtherPortal->TransformToGlobalCoordinate(position);
  UpdatePosition(step, position);
}

void Surface::PeriodicPortal::DoPeriodicPortation(const G4Step *step,
                                                  SingleSurface exitSurface) {
  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  DoPeriodicTransform(position, exitSurface);
  UpdatePosition(step, position);
  fLogger.WriteDebugInfo("Subworld: X:" + std::to_string(GetCurrentNX()) +
                         " Y: " + std::to_string(GetCurrentNY()));
}

Surface::PeriodicPortal::PortationType
Surface::PeriodicPortal::GetPortationType(SingleSurface surface) const {
  if (fIsPortal)
    return PortationType::ENTER;
  // Periodic exit at a surface
  if (surface == SingleSurface::X_UP and fCurrentNX < fMaxNX - 1)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::X_DOWN and fCurrentNX > 0)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::Y_UP and fCurrentNY < fMaxNY - 1)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::Y_DOWN and fCurrentNY > 0)
    return PortationType::PERIODIC;

  // Periodic exit at an edge
  if (surface == SingleSurface::X_UP_Y_UP and fCurrentNY < fMaxNX - 1 and
      fCurrentNY < fMaxNY - 1)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::X_UP_Y_DOWN and fCurrentNX < fMaxNX - 1 and
      fCurrentNY > 0)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::X_DOWN_Y_UP and fCurrentNX > 0 and
      fCurrentNY < fMaxNY - 1)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::X_DOWN_Y_DOWN and fCurrentNX > 0 and
      fCurrentNY > 0)
    return PortationType::PERIODIC;
  // exit at a Z Surface, also including corners.
  return PortationType::EXIT;
}

// Function to decide in which direction the particle left the volume
Surface::PeriodicPortal::SingleSurface
Surface::PeriodicPortal::GetNearestSurface(const G4Step *step) {
  G4VSolid *portalSolid = GetVolume()->GetLogicalVolume()->GetSolid();
  G4ThreeVector point = step->GetPostStepPoint()->GetPosition();
  point = TransformToLocalCoordinate(point);
  G4ThreeVector result = portalSolid->SurfaceNormal(point);

  fLogger.WriteDebugInfo("Point: x: " + std::to_string(point.x()) +
                         " y: " + std::to_string(point.y()) +
                         " z: " + std::to_string(point.z()));
  fLogger.WriteDebugInfo("SurfaceNormal: x: " + std::to_string(result.x()) +
                         " y: " + std::to_string(result.y()) +
                         " z: " + std::to_string(result.z()));

  auto IsZero = [](G4double a) {
    const G4double numeric_limit =
        std::numeric_limits<G4double>::epsilon() * 10;
    return fabs(a) < numeric_limit;
  };

  const G4bool IsZero_X = IsZero(result.x());
  const G4bool IsZero_Y = IsZero(result.y());
  const G4bool IsZero_Z = IsZero(result.z());

  // Case X or Y face
  if (result.x() > 0. and IsZero_Y and IsZero_Z) {
    return SingleSurface::X_UP;
  } else if (result.x() < 0. and IsZero_Y and IsZero_Z) {
    return SingleSurface::X_DOWN;
  } else if (IsZero_X and result.y() > 0. and IsZero_Z) {
    return SingleSurface::Y_UP;
  } else if (IsZero_X and result.y() < 0. and IsZero_Z) {
    return SingleSurface::Y_DOWN;
    // Case XY edges
  } else if (result.x() > 0. and result.y() > 0. and IsZero_Z) {
    return SingleSurface::X_UP_Y_UP;
  } else if (result.x() > 0. and result.y() < 0. and IsZero_Z) {
    return SingleSurface::X_UP_Y_DOWN;
  } else if (result.x() < 0. and result.y() > 0. and IsZero_Z) {
    return SingleSurface::X_DOWN_Y_UP;
  } else if (result.x() < 0. and result.y() < 0. and IsZero_Z) {
    return SingleSurface::X_DOWN_Y_DOWN;
    // Case including face Z after all other cases are excluded
  } else if (result.z() > 0.) {
    return SingleSurface::Z_UP;
  } else if (result.z() < 0.) {
    return SingleSurface::Z_DOWN;
  } else {
    exit(EXIT_FAILURE); // is a last check, should never happen
  }
}

void Surface::PeriodicPortal::DoPeriodicTransform(G4ThreeVector &vec,
                                                  SingleSurface surface) {
  G4VPhysicalVolume *volume = GetVolume();
  G4ThreeVector pMin, pMax;
  volume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  G4ThreeVector volumeSize = pMax - pMin;
  G4ThreeVector oldPosition = vec;
  switch (surface) {
  case SingleSurface::X_UP:
    vec.setX(oldPosition.x() - volumeSize.x());
    ++fCurrentNX;
    break;
  case SingleSurface::X_DOWN:
    vec.setX(oldPosition.x() + volumeSize.x());
    --fCurrentNX;
    break;
  case SingleSurface::Y_UP:
    vec.setY(oldPosition.y() - volumeSize.y());
    ++fCurrentNY;
    break;
  case SingleSurface::Y_DOWN:
    vec.setY(oldPosition.y() + volumeSize.y());
    --fCurrentNY;
    break;
  case SingleSurface::X_UP_Y_UP:
    vec.setX(oldPosition.x() - volumeSize.x());
    ++fCurrentNX;
    vec.setY(oldPosition.y() - volumeSize.y());
    ++fCurrentNY;
    break;
  case SingleSurface::X_UP_Y_DOWN:
    vec.setX(oldPosition.x() - volumeSize.x());
    ++fCurrentNX;
    vec.setY(oldPosition.y() + volumeSize.y());
    --fCurrentNY;
    break;
  case SingleSurface::X_DOWN_Y_UP:
    vec.setX(oldPosition.x() + volumeSize.x());
    --fCurrentNX;
    vec.setY(oldPosition.y() - volumeSize.y());
    ++fCurrentNY;
    break;
  case SingleSurface::X_DOWN_Y_DOWN:
    vec.setX(oldPosition.x() + volumeSize.x());
    --fCurrentNX;
    vec.setY(oldPosition.y() + volumeSize.y());
    --fCurrentNY;
    break;
  default:
    exit(EXIT_FAILURE); // should never happen
  }
  fLogger.WriteDebugInfo(
      "Periodic Transormation: NX: " + std::to_string(fCurrentNX) +
      " NY: " + std::to_string(fCurrentNY));
}

void Surface::PeriodicPortal::TransformSubworldToPortal(G4ThreeVector &vec) {
  G4ThreeVector pMinVol, pMinOtherVol, pMax;
  GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMinVol, pMax);
  const G4ThreeVector volumeDistance = pMax - pMinVol;
  fOtherPortal->GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(
      pMinOtherVol, pMax);
  const G4ThreeVector otherVolumeDistance = pMax - pMinOtherVol;
  const G4ThreeVector shiftedVec = vec + volumeDistance / 2.;
  vec.setX(shiftedVec.x() + fCurrentNX * volumeDistance.x() -
           otherVolumeDistance.x() / 2.);
  vec.setY(shiftedVec.y() + fCurrentNY * volumeDistance.y() -
           otherVolumeDistance.y() / 2.);

  vec.setZ(TransformZBetweenPortals(vec.z()));
}

void Surface::PeriodicPortal::TransformPortalToSubworld(G4ThreeVector &vec) {
  G4ThreeVector pMinVol, pMinOtherVol, pMax;
  GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMinVol, pMax);
  const G4ThreeVector volumeDistance = pMax - pMinVol;
  fOtherPortal->GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(
      pMinOtherVol, pMax);
  const G4ThreeVector otherVolumeDistance = pMax - pMinOtherVol;
  const G4ThreeVector shiftedVec = vec + volumeDistance / 2.;
  G4int NX = shiftedVec.x() / otherVolumeDistance.x();
  G4int NY = shiftedVec.y() / otherVolumeDistance.y();
  if (NX == fOtherPortal->fMaxNX)
    --NX;
  if (NY == fOtherPortal->fMaxNY)
    --NY;

  vec.setX(shiftedVec.x() - NX * otherVolumeDistance.x() -
           otherVolumeDistance.x() / 2.);
  vec.setY(shiftedVec.y() - NY * otherVolumeDistance.y() -
           otherVolumeDistance.y() / 2.);
  fOtherPortal->fCurrentNX = NX;
  fOtherPortal->fCurrentNY = NY;
  vec.setZ(TransformZBetweenPortals(vec.z()));
  fLogger.WriteDebugInfo("Subworld: X " + std::to_string(NX) + " Y " +
                         std::to_string(NY));
}

G4double Surface::PeriodicPortal::TransformZBetweenPortals(G4double val) {
  G4ThreeVector pMin, pMax;
  GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  G4double volumeDistanceZ = pMax.z() - pMin.z();
  fOtherPortal->GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(
      pMin, pMax);
  G4double otherVolumeDistanceZ = pMax.z() - pMin.z();
  val = val / volumeDistanceZ * otherVolumeDistanceZ;
  return val;
}

void Surface::PeriodicPortal::SetGrid(int nX, int nY) {
  fMaxNX = nX;
  fMaxNY = nY;
}

void Surface::PeriodicPortal::SetOtherPortal(
    Surface::PeriodicPortal *otherPortal) {
  fOtherPortal = otherPortal;
}
