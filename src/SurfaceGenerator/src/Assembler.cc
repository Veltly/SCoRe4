/**
 * @brief implementation of assembler class
 * @file Assembler.cc
 * @date 2023-06-01
 * @author C.Gruener
 */
#include "SurfaceGenerator/include/Assembler.hh"
#include <string>
#include "G4Box.hh"
#include "G4MultiUnion.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4Trd.hh"
#include "G4TriangularFacet.hh"
#include "G4VFacet.hh"
#include "SurfaceGenerator/include/FacetStore.hh"
#include "SurfaceGenerator/include/Storage.hh"

using Volumetype = Surface::SolidDescription::Solid;

Surface::Assembler::Assembler(FacetStore *store)
    : fLogger("Assembler"), fFacetStore(store) {}

void Surface::Assembler::Assemble() {
  auto *AssembledSolid = new G4MultiUnion;
  for (const auto &description : fDescription) {
    G4VSolid *newSolid = GetSingleSolid(description);
    if (fLogger.IsDebugInfoLvl()) {
      std::stringstream ss;
      ss << "\n";
      newSolid->StreamInfo(ss);
      ss << "Area:   " << newSolid->GetSurfaceArea() / (CLHEP::mm * CLHEP::mm)
         << " mm^2\n";
      ss << "Volume: "
         << newSolid->GetCubicVolume() / (CLHEP::mm * CLHEP::mm * CLHEP::mm)
         << " mm^3\n";
      ss << "-----------------------------------------------------------\n";
      ss << "-----------------------------------------------------------\n\n";
      fLogger.WriteDebugInfo(ss.str());
    }
    G4Transform3D transform{description.Transform};
    AssembledSolid->AddNode(*newSolid, transform);
    AddToFacetStore(description);
  }
  fSolid = AssembledSolid;
  fLogger.WriteInfo("Finished assemble");
}

void Surface::Assembler::AddToFacetStore(const SolidDescription &aDescription) {
  const G4VSolid *newSolid = GetSingleSolid(aDescription);
  G4Polyhedron polyhedron{*newSolid->CreatePolyhedron()};
  polyhedron.Transform(aDescription.Transform);
  G4int NVertices;
  G4Point3D Vertices[4];
  for (const auto FacetIndex : aDescription.OuterSurface) {
    polyhedron.GetFacet(FacetIndex, NVertices, Vertices);
    std::vector<G4ThreeVector> Tmp_Vertices;
    Tmp_Vertices.reserve(NVertices);
for (G4int i = 0; i < NVertices; ++i) {
      Tmp_Vertices.emplace_back(
          Vertices[i].x(), Vertices[i].y(), Vertices[i].z());
    }
    fFacetStore->AppendToFacetVector(new G4TriangularFacet{
        Tmp_Vertices[0], Tmp_Vertices[1], Tmp_Vertices[2], ABSOLUTE});
    if (NVertices == 4) {
      fFacetStore->AppendToFacetVector(new G4TriangularFacet{
          Tmp_Vertices[0], Tmp_Vertices[2], Tmp_Vertices[3], ABSOLUTE});
    }
  }
  delete newSolid;
}

G4VSolid *Surface::Assembler::GetSingleSolid(
    const SolidDescription &aDescription) {
  switch (aDescription.VolumeType) {
    case Volumetype::Box:
      return GetBox(aDescription);
    case Volumetype::Trd:
      return GetTrd(aDescription);
  }
}

G4VSolid *Surface::Assembler::GetBox(const SolidDescription &aDescription) {
  const G4String SolidName{GenerateSolidName(aDescription)};
  const G4double pX{aDescription.VolumeParameter.at(0)};
  const G4double pY{aDescription.VolumeParameter.at(1)};
  const G4double pZ{aDescription.VolumeParameter.at(2)};
  auto *newBox = new G4Box{SolidName, pX, pY, pZ};
  return newBox;
}

G4VSolid *Surface::Assembler::GetTrd(const SolidDescription &aDescription) {
  const G4String SolidName{GenerateSolidName(aDescription)};
  const G4double pXBottom{aDescription.VolumeParameter.at(0)};
  const G4double pXTop{aDescription.VolumeParameter.at(1)};
  const G4double pYBottom{aDescription.VolumeParameter.at(2)};
  const G4double pYTop{aDescription.VolumeParameter.at(3)};
  const G4double pHeight{aDescription.VolumeParameter.at(4)};
  auto *newTrd =
      new G4Trd{SolidName, pXBottom, pXTop, pYBottom, pYTop, pHeight};
  return newTrd;
}

G4String Surface::Assembler::GenerateSolidName(
    const SolidDescription &aDescription) {
  G4String name;
  for (const G4double string : aDescription.VolumeParameter) {
    name += std::to_string(string);
    name += "_";
  }
  const G4ThreeVector translation{aDescription.Transform.getTranslation()};
  name += std::to_string(translation.getX());
  name += "_";
  name += std::to_string(translation.getY());
  name += "_";
  name += std::to_string(translation.getZ());
  name += "_";
  return name;
}

G4MultiUnion* Surface::Assembler::GetSolid() const {
  if(fSolid == nullptr){
    fLogger.WriteError("GetSolid() called before solid is assembled.");
    throw std::logic_error("Solid not assembled.");
  }
  return fSolid;
}