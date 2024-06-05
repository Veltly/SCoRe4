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

Surface::PeriodicPortal::PeriodicPortal(G4String name,
                                        G4VPhysicalVolume *volume,
                                        G4ThreeVector &vec, G4int verbose)
    : VPortal(name, volume, PortalType::PeriodicPortal) {
  SetVerbose(verbose);
  SetGlobalCoord(vec);
};

void Surface::PeriodicPortal::DoPortation(const G4Step *step) {
  // select portation method by checking Grid and side of exit portal
  SingleSurface surface = GetNearestSurface(step);
  PortationType portationType = GetPortationType(surface);
  switch (portationType) {
  case PortationType::ENTER: {
    EnterPortal(step, surface);
    break;
  }
  case PortationType::EXIT: {
    ExitPortal(step, surface);
    break;
  }
  case PortationType::PERIODIC: {
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
}

Surface::PeriodicPortal::PortationType
Surface::PeriodicPortal::GetPortationType(SingleSurface surface) const {
  if (fIsPortal) {
    return PortationType::ENTER;
  }
  switch (surface) {
  case SingleSurface::X_UP:
  case SingleSurface::X_DOWN:
  case SingleSurface::Y_UP:
  case SingleSurface::Y_DOWN:
    return PortationType::PERIODIC;
    break;
  case SingleSurface::Z_UP:
  case SingleSurface::Z_DOWN:
    return PortationType::EXIT;
    break;
  default:
    exit(EXIT_FAILURE); // should never happen
  }
}

// Function to decide in which direction the particle left the volume
Surface::PeriodicPortal::SingleSurface
Surface::PeriodicPortal::GetNearestSurface(const G4Step *step) const {
  G4VSolid *portalSolid = GetVolume()->GetLogicalVolume()->GetSolid();
  G4ThreeVector result =
      portalSolid->SurfaceNormal(step->GetPostStepPoint()->GetPosition());
  if (result.x() > result.y() and result.x() > result.z()) {
    return SingleSurface::X_UP;
  } else if (result.x() < result.y() and result.x() < result.z()) {
    return SingleSurface::X_DOWN;
  } else if (result.y() > result.x() and result.y() > result.z()) {
    return SingleSurface::Y_UP;
  } else if (result.y() < result.x() and result.y() < result.z()) {
    return SingleSurface::Y_DOWN;
  } else if (result.z() > result.x() and result.z() > result.y()) {
    return SingleSurface::Z_UP;
  } else if (result.z() < result.x() and result.z() < result.y()) {
    return SingleSurface::Z_DOWN;
  } else {
    exit(EXIT_FAILURE); // is a last check, shoudl never happen
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
  default:
    exit(EXIT_FAILURE); // should never happen
  }
}

void Surface::PeriodicPortal::TransformSubworldToPortal(G4ThreeVector &vec) {
  // TO DO: Implementation

  vec.setZ(TransformZBetweenPortals(vec.z()));
}

void Surface::PeriodicPortal::TransformPortalToSubworld(G4ThreeVector &vec) {
  G4ThreeVector pMinVol, pMinOtherVol, pMax;
  GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMinVol, pMax);
  G4ThreeVector volumeDistance = pMax - pMinVol;
  fOtherPortal->GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(
      pMinOtherVol, pMax);
  G4ThreeVector otherVolumeDistance = pMax - pMinOtherVol;
  fCurrentNX = fMaxNX / 2. + vec.x() / otherVolumeDistance.x();
  fCurrentNY = fMaxNY / 2. + vec.y() / otherVolumeDistance.y();
  // TO DO: Implementation
  // fOtherPortal->fCurrentX and Y must be set

  vec.setZ(TransformZBetweenPortals(vec.z()));
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
