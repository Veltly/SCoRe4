//
//
//
//
//	author: C.Gruener
//
//
#include "../include/Assembler.hh"
#include "../../Service/Locator.hh"
#include "../include/Storage.hh"
#include "../include/FacetStore.hh"
#include <G4Box.hh>
#include <G4MultiUnion.hh>
#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4Trd.hh>
#include <G4VFacet.hh>

using Volumetype = Surface::SolidDescription::Solid;

void Surface::Assembler::Assemble() {
  G4MultiUnion *AssembledSolid = new G4MultiUnion;
  for (const auto &description : fDescription) {
    G4VSolid *newSolid = GetSingleSolid(description);
    G4Transform3D transform{description.Transform};
    AssembledSolid->AddNode(*newSolid, transform);
    AddToFacetStore(description);
  }
}

void Surface::Assembler::AddToFacetStore(const SolidDescription &aDescription) {
  Surface::FacetStore FacetStore = Surface::Locator::GetFacetStore();
  G4VSolid *newSolid = GetSingleSolid(aDescription);
  G4Polyhedron polyhedron{*newSolid->CreatePolyhedron()};
  polyhedron.Transform(aDescription.Transform);
  G4int NVertices;
  G4Point3D Vertices[4];
  for (auto FacetIndex : aDescription.OuterSurface) {
    polyhedron.GetFacet(FacetIndex, NVertices, Vertices);
    std::vector<G4ThreeVector> Tmp_Vertices;
    for (G4int i = 0; i < NVertices; ++i) {
      Tmp_Vertices.emplace_back(
          G4ThreeVector{Vertices[i].x(), Vertices[i].y(), Vertices[i].z()});
      FacetStore.AppendToFacetVector(G4TriangularFacet{
          Tmp_Vertices[0], Tmp_Vertices[1], Tmp_Vertices[2], ABSOLUTE});
    }
    delete newSolid;
  }
}

G4VSolid *
Surface::Assembler::GetSingleSolid(const SolidDescription &aDescription) {
  switch (aDescription.Volumetype) {
  case Volumetype::Box:
    return GetBox(aDescription);
  case Volumetype::Trd:
    return GetTrd(aDescription);
  }
}

G4VSolid *Surface::Assembler::GetBox(const SolidDescription &aDescription) {
  G4String SolidName{GenerateSolidName(aDescription)};
  G4double pX{aDescription.Volumeparameter.at(0)};
  G4double pY{aDescription.Volumeparameter.at(1)};
  G4double pZ{aDescription.Volumeparameter.at(2)};
  G4Box *newBox = new G4Box{SolidName, pX, pY, pZ};
  return newBox;
}

G4VSolid *Surface::Assembler::GetTrd(const SolidDescription &aDescription) {
  G4String SolidName{GenerateSolidName(aDescription)};
  G4double pXBottom{aDescription.Volumeparameter.at(0)};
  G4double pXTop{aDescription.Volumeparameter.at(1)};
  G4double pYBottom{aDescription.Volumeparameter.at(2)};
  G4double pYTop{aDescription.Volumeparameter.at(2)};
  G4double pHeight{aDescription.Volumeparameter.at(2)};
  G4Trd *newTrd =
      new G4Trd{SolidName, pXBottom, pXTop, pYBottom, pYTop, pHeight};
  return newTrd;
}
