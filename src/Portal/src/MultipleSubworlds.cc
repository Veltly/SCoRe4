// Author: C.Gruener
// Date: 24-06-11
// File: MultipleSubworlds

#include "../include/MultipleSubworld.hh"
#include "../include/SubworldGrid.hh"
#include "../include/VPortal.hh"

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include <G4Transform3D.hh>
#include <G4Types.hh>
#include <cstdlib>
#include <string>

Surface::MultipleSubworld::MultipleSubworld(const G4String name,
                                            G4VPhysicalVolume *volume,
                                            G4ThreeVector &vec,
                                            const G4int verbose,
                                            FacetStore *facetStore)
    : VPortal(name, volume, PortalType::MultipleSubworld),
      fLogger({"MultiplePortal", verbose}), fIsPortal(false),
      fSubworldGrid(nullptr), fFacetStore(facetStore) {
  SetGlobalCoord(vec);
};

Surface::MultipleSubworld::MultipleSubworld(const G4String name,
                                            G4VPhysicalVolume *volume,
                                            G4Transform3D transform,
                                            const G4int verbose,
                                            FacetStore *facetStore)
    : VPortal(name, volume, PortalType::MultipleSubworld),
      fLogger({"MultiplePortal", verbose}), fIsPortal(false),
      fSubworldGrid(nullptr), fFacetStore(facetStore) {
  const G4ThreeVector vec = transform.getTranslation();
  SetGlobalCoord(vec);
};

Surface::MultipleSubworld::~MultipleSubworld() {
  if (fSubworldGrid !=
      nullptr) // substitution for a shared pointer, be carefull, will be
               // deleted if any subworld is deleted.
    delete fSubworldGrid;
  fSubworldGrid = nullptr;
};

void Surface::MultipleSubworld::DoPortation(const G4Step *step) {
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

void Surface::MultipleSubworld::EnterPortal(const G4Step *step,
                                            const SingleSurface enterSurface) {

  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  // fLogger.WriteDebugInfo(
  //     "Current position    x: " + std::to_string(position.x()) + " y: " +
  //     std::to_string(position.y()) + " z: " + std::to_string(position.z()));
  TransformToLocalCoordinate(position);
  // fLogger.WriteDebugInfo(
  //     "Local Coordinate    x: " + std::to_string(position.x()) + " y: " +
  //     std::to_string(position.y()) + " z: " + std::to_string(position.z()));
  TransformPortalToSubworld(position);
  // fLogger.WriteDebugInfo(
  //     "Subworld Coordinate x: " + std::to_string(position.x()) + " y: " +
  //     std::to_string(position.y()) + " z: " + std::to_string(position.z()));
  fSubworldGrid->GetSubworld()->TransformToGlobalCoordinate(position);
  // MultipleSubworld *sub = fSubworldGrid->GetSubworld();
  //  fLogger.WriteDebugInfo("Subworld name: " + sub->GetName());
  //  fLogger.WriteDebugInfo(
  //      "Global Coordinate   x: " + std::to_string(position.x()) + " y: " +
  //      std::to_string(position.y()) + " z: " + std::to_string(position.z()));
  UpdatePosition(step, position);
}

void Surface::MultipleSubworld::ExitPortal(const G4Step *step,
                                           const SingleSurface exitSurface) {
  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  TransformToLocalCoordinate(position);
  TransformSubworldToPortal(position);
  fPortal->TransformToGlobalCoordinate(position);
  UpdatePosition(step, position);
}

void Surface::MultipleSubworld::DoPeriodicPortation(
    const G4Step *step, const SingleSurface exitSurface) {
  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  DoPeriodicTransform(position, exitSurface);
  UpdatePosition(step, position);
  LoggCurrentStatus();
}

Surface::MultipleSubworld::PortationType
Surface::MultipleSubworld::GetPortationType(const SingleSurface surface) {
  if (fIsPortal)
    return PortationType::ENTER;
  const G4int currentNX = fSubworldGrid->CurrentPosX();
  const G4int currentNY = fSubworldGrid->CurrentPosY();
  const G4int maxNX = fSubworldGrid->MaxX();
  const G4int maxNY = fSubworldGrid->MaxY();
  // Periodic exit at a surface
  if (surface == SingleSurface::X_UP and currentNX < maxNX - 1)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::X_DOWN and currentNX > 0)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::Y_UP and currentNY < maxNY - 1)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::Y_DOWN and currentNY > 0)
    return PortationType::PERIODIC;

  // Periodic exit at an edge
  if (surface == SingleSurface::X_UP_Y_UP and currentNY < maxNX - 1 and
      currentNY < maxNY - 1)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::X_UP_Y_DOWN and currentNX < maxNX - 1 and
      currentNY > 0)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::X_DOWN_Y_UP and currentNX > 0 and
      currentNY < maxNY - 1)
    return PortationType::PERIODIC;
  if (surface == SingleSurface::X_DOWN_Y_DOWN and currentNX > 0 and
      currentNY > 0)
    return PortationType::PERIODIC;
  // exit at a Z Surface, also including corners.
  return PortationType::EXIT;
}

// Function to decide in which direction the particle left the volume
Surface::MultipleSubworld::SingleSurface
Surface::MultipleSubworld::GetNearestSurface(const G4Step *step) {
  G4VSolid *portalSolid = GetVolume()->GetLogicalVolume()->GetSolid();
  G4ThreeVector point = step->GetPostStepPoint()->GetPosition();
  TransformToLocalCoordinate(point);
  const G4ThreeVector result = portalSolid->SurfaceNormal(point);
  const G4ThreeVector direction =
      step->GetPostStepPoint()->GetMomentumDirection();

  fLogger.WriteDebugInfo("Point for finding surface normal", point);
  fLogger.WriteDebugInfo("Surface normal                  ", result);
  fLogger.WriteDebugInfo("Momentum direction              ", direction);

  auto IsZero = [](const G4double a) {
    const G4double numeric_limit =
        std::numeric_limits<G4double>::epsilon() * 10;
    return fabs(a) < numeric_limit;
  };

  const G4bool IsZero_X = IsZero(result.x());
  const G4bool IsZero_Y = IsZero(result.y());
  const G4bool IsZero_Z = IsZero(result.z());

  SingleSurface X, Y, Z;

  // X
  if (IsZero_X) { // extra case because of comparison double with 0.
    X = SingleSurface::X_SAME;
  } else if (result.x() > 0. and direction.x() > 0.) {
    X = SingleSurface::X_UP;
  } else if (result.x() < 0. and direction.x() < 0.) {
    X = SingleSurface::X_DOWN;
  } else {
    X = SingleSurface::X_SAME;
  }

  // Y
  if (IsZero_Y) {
    Y = SingleSurface::Y_SAME;
  } else if (result.y() > 0. and direction.y() > 0.) {
    Y = SingleSurface::Y_UP;
  } else if (result.y() < 0. and direction.y() < 0.) {
    Y = SingleSurface::Y_DOWN;
  } else {
    Y = SingleSurface::Y_SAME;
  }

  // Z
  if (IsZero_Z) {
    Z = SingleSurface::Z_SAME;
  } else if (result.z() > 0. and direction.z() > 0.) {
    Z = SingleSurface::Z_UP;
  } else if (result.z() < 0. and direction.z() < 0.) {
    Z = SingleSurface::Z_DOWN;
  } else {
    Z = SingleSurface::Z_SAME;
  }

  if (Z == SingleSurface::Z_UP) {
    return SingleSurface::Z_UP;
  } else if (Z == SingleSurface::Z_DOWN) {
    return SingleSurface::Z_DOWN;
  } else if (X == SingleSurface::X_UP and Y == SingleSurface::Y_UP) {
    return SingleSurface::X_UP_Y_UP;
  } else if (X == SingleSurface::X_UP and Y == SingleSurface::Y_DOWN) {
    return SingleSurface::X_UP_Y_DOWN;
  } else if (X == SingleSurface::X_DOWN and Y == SingleSurface::Y_UP) {
    return SingleSurface::X_DOWN_Y_UP;
  } else if (X == SingleSurface::X_DOWN and Y == SingleSurface::Y_DOWN) {
    return SingleSurface::X_DOWN_Y_DOWN;
  } else if (X == SingleSurface::X_UP) {
    return SingleSurface::X_UP;
  } else if (X == SingleSurface::X_DOWN) {
    return SingleSurface::X_DOWN;
  } else if (Y == SingleSurface::Y_UP) {
    return SingleSurface::Y_UP;
  } else if (Y == SingleSurface::Y_DOWN) {
    return SingleSurface::Y_DOWN;
  } else {
    return SingleSurface::Z_SAME;
  }
}

void Surface::MultipleSubworld::DoPeriodicTransform(
    G4ThreeVector &vec, const SingleSurface surface) {
  G4VPhysicalVolume *volume = GetVolume();
  G4ThreeVector pMin, pMax;
  volume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  const G4ThreeVector volumeSize = pMax - pMin;
  TransformToLocalCoordinate(vec); // transform to local coord
  const G4ThreeVector oldPosition = vec;
  // I assume that all subworlds have the same dimension
  switch (surface) {
  case SingleSurface::X_UP:
    vec.setX(oldPosition.x() - volumeSize.x());
    fSubworldGrid->IncrX();
    break;
  case SingleSurface::X_DOWN:
    vec.setX(oldPosition.x() + volumeSize.x());
    fSubworldGrid->DecrX();
    break;
  case SingleSurface::Y_UP:
    vec.setY(oldPosition.y() - volumeSize.y());
    fSubworldGrid->IncrY();
    break;
  case SingleSurface::Y_DOWN:
    vec.setY(oldPosition.y() + volumeSize.y());
    fSubworldGrid->DecrY();
    break;
  case SingleSurface::X_UP_Y_UP:
    vec.setX(oldPosition.x() - volumeSize.x());
    fSubworldGrid->IncrX();
    vec.setY(oldPosition.y() - volumeSize.y());
    fSubworldGrid->IncrY();
    break;
  case SingleSurface::X_UP_Y_DOWN:
    vec.setX(oldPosition.x() - volumeSize.x());
    fSubworldGrid->IncrX();
    vec.setY(oldPosition.y() + volumeSize.y());
    fSubworldGrid->DecrY();
    break;
  case SingleSurface::X_DOWN_Y_UP:
    vec.setX(oldPosition.x() + volumeSize.x());
    fSubworldGrid->DecrX();
    vec.setY(oldPosition.y() - volumeSize.y());
    fSubworldGrid->IncrY();
    break;
  case SingleSurface::X_DOWN_Y_DOWN:
    vec.setX(oldPosition.x() + volumeSize.x());
    fSubworldGrid->DecrX();
    vec.setY(oldPosition.y() + volumeSize.y());
    fSubworldGrid->DecrY();
    break;
  default:
    exit(EXIT_FAILURE); // should never happen
  }
  fSubworldGrid->GetSubworld()->TransformToGlobalCoordinate(
      vec); // transform to coord of new subworld
  const std::string subworldName = fSubworldGrid->GetSubworld()->GetName();
  LoggCurrentStatus();
}

void Surface::MultipleSubworld::TransformSubworldToPortal(G4ThreeVector &vec) {
  G4ThreeVector pMinVol, pMinOtherVol, pMax;
  GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMinVol, pMax);
  const G4ThreeVector volumeDistance = pMax - pMinVol;
  fPortal->GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(
      pMinOtherVol, pMax);
  const G4ThreeVector otherVolumeDistance = pMax - pMinOtherVol;
  const G4ThreeVector shiftedVec = vec + volumeDistance / 2.;
  vec.setX(shiftedVec.x() + fSubworldGrid->CurrentPosX() * volumeDistance.x() -
           otherVolumeDistance.x() / 2.);
  vec.setY(shiftedVec.y() + fSubworldGrid->CurrentPosY() * volumeDistance.y() -
           otherVolumeDistance.y() / 2.);

  vec.setZ(TransformZBetweenPortals(vec.z()));
}

void Surface::MultipleSubworld::TransformPortalToSubworld(G4ThreeVector &vec) {
  G4ThreeVector pMinVol, pMinOtherVol, pMax;
  GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMinVol, pMax);
  const G4ThreeVector volumeDistance = pMax - pMinVol;
  fPortal->GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(
      pMinOtherVol, pMax);
  const G4ThreeVector otherVolumeDistance = pMax - pMinOtherVol;
  const G4ThreeVector shiftedVec = vec + volumeDistance / 2.;
  G4double divNX = shiftedVec.x() / otherVolumeDistance.x();
  G4double divNY = shiftedVec.y() / otherVolumeDistance.y();
  G4int NX = shiftedVec.x() / otherVolumeDistance.x();
  G4int NY = shiftedVec.y() / otherVolumeDistance.y();
  fLogger.WriteDebugInfo("Calculate gridposition x: " + std::to_string(divNX) +
                         " y: " + std::to_string(divNY) + " rounded x: " +
                         std::to_string(NX) + " y: " + std::to_string(NY));
  if (NX == fSubworldGrid->MaxX())
    --NX;
  if (NY == fSubworldGrid->MaxY())
    --NY;
  vec.setX(shiftedVec.x() - NX * otherVolumeDistance.x() -
           otherVolumeDistance.x() / 2.);
  vec.setY(shiftedVec.y() - NY * otherVolumeDistance.y() -
           otherVolumeDistance.y() / 2.);
  fSubworldGrid->SetCurrentX(NX);
  fSubworldGrid->SetCurrentY(NY);
  vec.setZ(TransformZBetweenPortals(vec.z()));
  LoggCurrentStatus();
}

G4double
Surface::MultipleSubworld::TransformZBetweenPortals(const G4double val) {
  G4ThreeVector pMin, pMax;
  GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  G4double volumeDistanceZ = pMax.z() - pMin.z();
  fPortal->GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin,
                                                                       pMax);
  G4double otherVolumeDistanceZ = pMax.z() - pMin.z();
  return val / volumeDistanceZ * otherVolumeDistanceZ;
}

void Surface::MultipleSubworld::SetGrid(const G4int sizeX, const G4int sizeY,
                                        const G4int verbose) {
  if (fSubworldGrid != nullptr) {
    fLogger.WriteError("Subworld Grid already set!");
    exit(EXIT_FAILURE);
  }
  fSubworldGrid = new SubworldGrid<MultipleSubworld>(sizeX, sizeY, verbose);
}

void Surface::MultipleSubworld::SetGrid(
    Surface::SubworldGrid<MultipleSubworld> *grid) {
  if (fSubworldGrid != nullptr) {
    fLogger.WriteError("Grid already set");
    exit(EXIT_FAILURE);
  }
  fSubworldGrid = grid;
}

void Surface::MultipleSubworld::SetOtherPortal(
    Surface::MultipleSubworld *otherPortal) {
  fPortal = otherPortal;
}

void Surface::MultipleSubworld::AddSubworldToGrid(
    const G4int x, const G4int y, Surface::MultipleSubworld *subworld) {
  if (fSubworldGrid == nullptr)
    exit(EXIT_FAILURE);
  if (subworld->GetSubworldGrid() != nullptr)
    exit(EXIT_FAILURE);
  fSubworldGrid->SetSubworld(x, y, subworld);
}

void Surface::MultipleSubworld::SetSubworldEdge(const G4double edgeX,
                                                const G4double edgeY,
                                                const G4double edgeZ) {
  fEdgeX = edgeX;
  fEdgeY = edgeY;
  fEdgeZ = edgeZ;
}

void Surface::MultipleSubworld::LoggCurrentStatus() {
  const std::string name = fSubworldGrid->GetSubworld()->GetName();
  const std::string nX = std::to_string(fSubworldGrid->CurrentPosX());
  const std::string nY = std::to_string(fSubworldGrid->CurrentPosY());
  fLogger.WriteInfo("Current subworld is: " + name + " at X: " + nX +
                    " Y: " + nY);
}
