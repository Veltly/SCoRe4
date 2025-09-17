/**
 * @brief Implementation of class MultipleSubworld
 * @author C.Gruener
 * @date 2024-06-11
 * @file MultipleSubworld.cc
 */

#include <cstdlib>
#include <string>

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4Types.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"
#include "Portal/include/MultipleSubworld.hh"
#include "Portal/include/SubworldGrid.hh"
#include "Portal/include/VPortal.hh"

Surface::MultipleSubworld::MultipleSubworld(const G4String &name,
                                            G4VPhysicalVolume *volume,
                                            const G4ThreeVector &vec,
                                            const VerboseLevel verbose,
                                            FacetStore *facetStore)
    : VPortal("MultipleSubworld_" + name, volume, PortalType::MultipleSubworld,
              verbose),
      fIsPortal(false),
      fSubworldGrid(nullptr),
      fFacetStore(facetStore) {
  SetGlobalCoord(vec);
}

Surface::MultipleSubworld::MultipleSubworld(const G4String &name,
                                            G4VPhysicalVolume *volume,
                                            const G4Transform3D transform,
                                            const VerboseLevel verbose,
                                            FacetStore *facetStore)
    : MultipleSubworld(name, volume, transform.getTranslation(), verbose, facetStore){}

Surface::MultipleSubworld::~MultipleSubworld() {
  delete fSubworldGrid;
  fSubworldGrid = nullptr;
}

/**
 * @brief Does the portation of the particle of the post step point
 * @param step
 */
void Surface::MultipleSubworld::DoPortation(G4Step *step) {
  const Direction direction{GetNearestSurface(step)}; //select direction the particle is exiting the volume
  const PortationType portationType{GetPortationType(direction)}; //select portation type
  switch (portationType) {
    case PortationType::ENTER: {
      fLogger.WriteDebugInfo("Doing portation of type: Enter");
      EnterPortal(step);
      break;
    }
    case PortationType::EXIT: {
      fLogger.WriteDebugInfo("Doing portation of type: Exit");
      ExitPortal(step);
      break;
    }
    case PortationType::PERIODIC: {
      fLogger.WriteDebugInfo("Doing portation of type: Periodic");
      DoPeriodicPortation(step, direction);
      break;
    }
  }
}

/**
 * @brief Portation method for entering the portal
 * @param step
 */
void Surface::MultipleSubworld::EnterPortal(G4Step *step) {
  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  fLogger.WriteDebugInfo("Current position", position);
  TransformToLocalCoordinate(position);
  fLogger.WriteDebugInfo("Local Coordinate", position);
  TransformPortalToSubworld(position);
  fLogger.WriteDebugInfo("Subworld Coordinate", position);
  fSubworldGrid->GetSubworld()->TransformToGlobalCoordinate(position);

  if(fLogger.IsDebugInfoLvl()) {
    MultipleSubworld *sub = fSubworldGrid->GetSubworld();
    fLogger.WriteDebugInfo("Subworld name: " + sub->GetName());
    fLogger.WriteDebugInfo("Global Coordinate", position);
  }
  UpdatePosition(step, position);
}

/**
 * @brief Portation method for exiting the portal
 * @param step
 */
void Surface::MultipleSubworld::ExitPortal(G4Step *step) {
  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  TransformToLocalCoordinate(position);
  TransformSubworldToPortal(position);
  fPortal->TransformToGlobalCoordinate(position);
  UpdatePosition(step, position);
}

/**
 * @brief Portation method for doing a step on the subworld grid
 * @param step
 * @param exitDirection exit direction of subworld
 */
void Surface::MultipleSubworld::DoPeriodicPortation(
    G4Step *step, const Direction exitDirection) {
  G4ThreeVector position = step->GetPostStepPoint()->GetPosition();
  DoPeriodicTransform(position, exitDirection);
  UpdatePosition(step, position);
  fLogger.WriteDebugInfo([this] {return CurrentStatusString();});
}

Surface::MultipleSubworld::PortationType
Surface::MultipleSubworld::GetPortationType(const Direction surface) {
  if (fIsPortal) return PortationType::ENTER;
  const G4int currentNX = fSubworldGrid->CurrentPosX();
  const G4int currentNY = fSubworldGrid->CurrentPosY();
  const G4int maxNX = fSubworldGrid->MaxX();
  const G4int maxNY = fSubworldGrid->MaxY();
  // Periodic exit at a surface
  if (surface == Direction::X_UP && currentNX < maxNX - 1)
    return PortationType::PERIODIC;
  if (surface == Direction::X_DOWN && currentNX > 0)
    return PortationType::PERIODIC;
  if (surface == Direction::Y_UP && currentNY < maxNY - 1)
    return PortationType::PERIODIC;
  if (surface == Direction::Y_DOWN && currentNY > 0)
    return PortationType::PERIODIC;

  // Periodic exit at an edge
  if (surface == Direction::X_UP_Y_UP && currentNY < maxNX - 1 &&
      currentNY < maxNY - 1)
    return PortationType::PERIODIC;
  if (surface == Direction::X_UP_Y_DOWN && currentNX < maxNX - 1 &&
      currentNY > 0)
    return PortationType::PERIODIC;
  if (surface == Direction::X_DOWN_Y_UP && currentNX > 0 &&
      currentNY < maxNY - 1)
    return PortationType::PERIODIC;
  if (surface == Direction::X_DOWN_Y_DOWN && currentNX > 0 && currentNY > 0)
    return PortationType::PERIODIC;
  // exit at a Z Surface, also including corners.
  return PortationType::EXIT;
}
// Function to decide in which direction the particle left the volume
/**
 * @brief Function to decide in which direction the particle left the volume
 * @param step
 * @return Direction
 */
Surface::MultipleSubworld::Direction
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

  Direction X, Y, Z;

  // X
  if (IsZero_X) {  // extra case because of comparison double with 0.
    X = Direction::X_SAME;
  } else if (result.x() > 0. && direction.x() > 0.) {
    X = Direction::X_UP;
  } else if (result.x() < 0. && direction.x() < 0.) {
    X = Direction::X_DOWN;
  } else {
    X = Direction::X_SAME;
  }

  // Y
  if (IsZero_Y) {
    Y = Direction::Y_SAME;
  } else if (result.y() > 0. && direction.y() > 0.) {
    Y = Direction::Y_UP;
  } else if (result.y() < 0. && direction.y() < 0.) {
    Y = Direction::Y_DOWN;
  } else {
    Y = Direction::Y_SAME;
  }

  // Z
  if (IsZero_Z) {
    Z = Direction::Z_SAME;
  } else if (result.z() > 0. && direction.z() > 0.) {
    Z = Direction::Z_UP;
  } else if (result.z() < 0. && direction.z() < 0.) {
    Z = Direction::Z_DOWN;
  } else {
    Z = Direction::Z_SAME;
  }

  if (Z == Direction::Z_UP) {
    return Direction::Z_UP;
  } else if (Z == Direction::Z_DOWN) {
    return Direction::Z_DOWN;
  } else if (X == Direction::X_UP && Y == Direction::Y_UP) {
    return Direction::X_UP_Y_UP;
  } else if (X == Direction::X_UP && Y == Direction::Y_DOWN) {
    return Direction::X_UP_Y_DOWN;
  } else if (X == Direction::X_DOWN && Y == Direction::Y_UP) {
    return Direction::X_DOWN_Y_UP;
  } else if (X == Direction::X_DOWN && Y == Direction::Y_DOWN) {
    return Direction::X_DOWN_Y_DOWN;
  } else if (X == Direction::X_UP) {
    return Direction::X_UP;
  } else if (X == Direction::X_DOWN) {
    return Direction::X_DOWN;
  } else if (Y == Direction::Y_UP) {
    return Direction::Y_UP;
  } else if (Y == Direction::Y_DOWN) {
    return Direction::Y_DOWN;
  } else {
    return Direction::Z_SAME;
  }
}

void Surface::MultipleSubworld::DoPeriodicTransform(
    G4ThreeVector &vec, const Direction surface) {
  G4VPhysicalVolume *volume = GetVolume();
  G4ThreeVector pMin, pMax;
  volume->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  const G4ThreeVector volumeSize = pMax - pMin;
  TransformToLocalCoordinate(vec);  // transform to local coord
  const G4ThreeVector oldPosition = vec;
  // I assume that all subworlds have the same dimension
  switch (surface) {
    case Direction::X_UP:
      vec.setX(oldPosition.x() - volumeSize.x());
      fSubworldGrid->IncrX();
      break;
    case Direction::X_DOWN:
      vec.setX(oldPosition.x() + volumeSize.x());
      fSubworldGrid->DecrX();
      break;
    case Direction::Y_UP:
      vec.setY(oldPosition.y() - volumeSize.y());
      fSubworldGrid->IncrY();
      break;
    case Direction::Y_DOWN:
      vec.setY(oldPosition.y() + volumeSize.y());
      fSubworldGrid->DecrY();
      break;
    case Direction::X_UP_Y_UP:
      vec.setX(oldPosition.x() - volumeSize.x());
      fSubworldGrid->IncrX();
      vec.setY(oldPosition.y() - volumeSize.y());
      fSubworldGrid->IncrY();
      break;
    case Direction::X_UP_Y_DOWN:
      vec.setX(oldPosition.x() - volumeSize.x());
      fSubworldGrid->IncrX();
      vec.setY(oldPosition.y() + volumeSize.y());
      fSubworldGrid->DecrY();
      break;
    case Direction::X_DOWN_Y_UP:
      vec.setX(oldPosition.x() + volumeSize.x());
      fSubworldGrid->DecrX();
      vec.setY(oldPosition.y() - volumeSize.y());
      fSubworldGrid->IncrY();
      break;
    case Direction::X_DOWN_Y_DOWN:
      vec.setX(oldPosition.x() + volumeSize.x());
      fSubworldGrid->DecrX();
      vec.setY(oldPosition.y() + volumeSize.y());
      fSubworldGrid->DecrY();
      break;
    default:
      exit(EXIT_FAILURE);  // should never happen
  }
  fSubworldGrid->GetSubworld()->TransformToGlobalCoordinate(
      vec);  // transform to coord of new subworld
  const std::string subworldName = fSubworldGrid->GetSubworld()->GetName();
  fLogger.WriteDebugInfo([this] {return CurrentStatusString();});
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
  const G4double divNX = shiftedVec.x() / otherVolumeDistance.x();
  const G4double divNY = shiftedVec.y() / otherVolumeDistance.y();
  G4int NX = std::floor(divNX);
  G4int NY = std::floor(divNY);

  fLogger.WriteDebugInfo("Calculate grid position x: " + std::to_string(divNX) +
                         " y: " + std::to_string(divNY) + " rounded x: " +
                         std::to_string(NX) + " y: " + std::to_string(NY));
  if (NX == fSubworldGrid->MaxX()) --NX;
  if (NY == fSubworldGrid->MaxY()) --NY;
  vec.setX(shiftedVec.x() - NX * otherVolumeDistance.x() -
           otherVolumeDistance.x() / 2.);
  vec.setY(shiftedVec.y() - NY * otherVolumeDistance.y() -
           otherVolumeDistance.y() / 2.);
  fSubworldGrid->SetCurrentX(NX);
  fSubworldGrid->SetCurrentY(NY);
  vec.setZ(TransformZBetweenPortals(vec.z()));
  fLogger.WriteDebugInfo([this] {return CurrentStatusString();});
}

G4double Surface::MultipleSubworld::TransformZBetweenPortals(
    const G4double val) {
  G4ThreeVector pMin, pMax;
  GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin, pMax);
  G4double volumeDistanceZ = pMax.z() - pMin.z();
  fPortal->GetVolume()->GetLogicalVolume()->GetSolid()->BoundingLimits(pMin,
                                                                       pMax);
  G4double otherVolumeDistanceZ = pMax.z() - pMin.z();
  return val / volumeDistanceZ * otherVolumeDistanceZ;
}

void Surface::MultipleSubworld::SetGrid(const G4int sizeX, const G4int sizeY,
                                        const VerboseLevel verbose) {
  if (fSubworldGrid != nullptr) {
    fLogger.WriteError("Subworld grid already set!");
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
  if (fSubworldGrid == nullptr) exit(EXIT_FAILURE);
  if (subworld->GetSubworldGrid() != nullptr) exit(EXIT_FAILURE);
  fSubworldGrid->SetSubworld(x, y, subworld);
}

void Surface::MultipleSubworld::SetSubworldEdge(const G4double edgeX,
                                                const G4double edgeY,
                                                const G4double edgeZ) {
  fEdgeX = edgeX;
  fEdgeY = edgeY;
  fEdgeZ = edgeZ;
}

std::string Surface::MultipleSubworld::CurrentStatusString() const {
  std::stringstream ss;
  ss << "Current subworld is: " << fSubworldGrid->GetSubworld()->GetName();
  ss << " at X: " << fSubworldGrid->CurrentPosX();
  ss << " Y: " << fSubworldGrid->CurrentPosY();
  return ss.str();
}

void Surface::MultipleSubworld::SetFacetStore(FacetStore *store) {
  fFacetStore = store;
}
