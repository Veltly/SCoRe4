//
//
//
//
//  author C.Gruener
//
//

#include "../include/FacetStore.hh"
#include <G4TriangularFacet.hh>
#include <Randomize.hh>
#include <G4UImanager.hh>
#include <G4UIcommand.hh>

void Surface::FacetStore::CloseFacetStore(){
  if(GetIsStoreClosed()){
    return;
  }
  CalculateFacetProbability(); 
  fClosed = true;
}

void Surface::FacetStore::CalculateFacetProbability(){
  G4double TotalArea{0};
  for(auto& facet:fFacetVector){
    TotalArea += facet.GetArea();
  }
  fFacetProbability.reserve(fFacetVector.size());
  G4double AreaTmp{0};
  //Calculates probability and sums it up
  for(auto& facet:fFacetVector){
    AreaTmp += facet.GetArea();
    fFacetProbability.emplace_back(AreaTmp / TotalArea);
  } 
}


G4ThreeVector Surface::FacetStore::GetRandomPoint() const {
  G4double random = G4UniformRand();
  for (size_t i = 0; i < fFacetProbability.size(); ++i) {
    if (random <= fFacetProbability[i]) {
      return fFacetVector[i].GetPointOnFace();
    }
  }
  return G4ThreeVector{0, 0, 0};
}

Surface::FacetStore::FacetEdges Surface::FacetStore::GetFacetLines(const G4TriangularFacet& aFacet){
    G4String (*toStr)(G4double) = G4UIcommand::ConvertToString;
    G4String edgeAB, edgeBC, edgeCA;
    auto vertexA = aFacet.GetVertex(0);
    auto vertexB = aFacet.GetVertex(1);
    auto vertexC = aFacet.GetVertex(2);
    edgeAB = toStr(vertexA.getX()) + " " + toStr(vertexA.getY()) + " " +
             toStr(vertexA.getZ()) + " " + toStr(vertexB.getX()) + " " +
             toStr(vertexB.getY()) + " " + toStr(vertexB.getZ()) + " mm";
    edgeBC = toStr(vertexB.getX()) + " " + toStr(vertexB.getY()) + " " +
             toStr(vertexB.getZ()) + " " + toStr(vertexC.getX()) + " " +
             toStr(vertexC.getY()) + " " + toStr(vertexC.getZ()) + " mm";
    edgeCA = toStr(vertexC.getX()) + " " + toStr(vertexC.getY()) + " " +
             toStr(vertexC.getZ()) + " " + toStr(vertexA.getX()) + " " +
             toStr(vertexA.getY()) + " " + toStr(vertexA.getZ()) + " mm";
    return {edgeAB, edgeBC, edgeCA};
}


void Surface::FacetStore::DrawFacets() {
  G4UImanager *UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/vis/verbose error");
  UI->ApplyCommand("/vis/set/colour 1 0 0");
  UI->ApplyCommand("/vis/set/linewidth 1.5");

  for (const auto &facet : fFacetVector) {
    FacetEdges edges{GetFacetLines(facet)};
    UI->ApplyCommand("/vis/scene/add/line " + edges.edgeAB);
    UI->ApplyCommand("/vis/scene/add/line " + edges.edgeBC);
    UI->ApplyCommand("/vis/scene/add/line " + edges.edgeCA);
  }

  UI->ApplyCommand("/vis/set/lineWidth");
  UI->ApplyCommand("/vis/set/colour");
  UI->ApplyCommand("/vis/verbose");
}

void Surface::FacetStore::AppendToFacetVector(const G4TriangularFacet& aFacet){
  fFacetVector.push_back((aFacet));
}
