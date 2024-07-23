// Copyright [2024] C.Gruener
// Date: 23-06-01
// File:

#include "SurfaceGenerator/include/FacetStore.hh"

#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4TriangularFacet.hh"
#include "G4Types.hh"
#include "G4UIcommand.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

void Surface::FacetStore::CloseFacetStore() {
  if (GetIsStoreClosed()) {
    return;
  }
  CalculateFacetProbability();
  fClosed = true;
  fLogger.WriteInfo(StreamInfo().str());
}

void Surface::FacetStore::CalculateFacetProbability() {
  G4double TotalArea{0};
  for (auto &facet : fFacetVector) {
    TotalArea += facet->GetArea();
  }
  fFacetProbability.reserve(fFacetVector.size());
  G4double AreaTmp{0};
  // Calculates probability and sums it up
  for (auto &facet : fFacetVector) {
    AreaTmp += facet->GetArea();
    fFacetProbability.emplace_back(AreaTmp / TotalArea);
  }
}

G4ThreeVector Surface::FacetStore::GetRandomPoint() const {
  G4double random = G4UniformRand();
  for (size_t i = 0; i < fFacetProbability.size(); ++i) {
    if (random <= fFacetProbability[i]) {
      auto point = fFacetVector[i]->GetPointOnFace();
      point = fTransform + point;
      return point;
    }
  }
  exit(EXIT_FAILURE);
  return G4ThreeVector{0, 0, 0};
}

G4ThreeVector Surface::FacetStore::GetRandomPoint(
    G4ThreeVector &surfaceNormal) {
  G4double random = G4UniformRand();
  for (size_t i = 0; i < fFacetProbability.size(); ++i) {
    if (random <= fFacetProbability[i]) {
      auto point = fFacetVector[i]->GetPointOnFace();
      surfaceNormal = fFacetVector[i]->GetSurfaceNormal();
      surfaceNormal /= surfaceNormal.r();
      point = fTransform + point;
      return point;
    }
  }
  exit(EXIT_FAILURE);
  return G4ThreeVector{0, 0, 0};
}

Surface::FacetStore::FacetEdges Surface::FacetStore::GetFacetLines(
    const G4TriangularFacet &aFacet) const {
  G4String (*toStr)(G4double) = G4UIcommand::ConvertToString;
  G4String edgeAB, edgeBC, edgeCA, edgeAMid;
  auto vertexA = aFacet.GetVertex(0);
  auto vertexB = aFacet.GetVertex(1);
  auto vertexC = aFacet.GetVertex(2);
  auto vertexMid = (vertexB + vertexC) / 2.;
  edgeAB = toStr(vertexA.getX()) + " " + toStr(vertexA.getY()) + " " +
           toStr(vertexA.getZ()) + " " + toStr(vertexB.getX()) + " " +
           toStr(vertexB.getY()) + " " + toStr(vertexB.getZ()) + " mm";
  edgeBC = toStr(vertexB.getX()) + " " + toStr(vertexB.getY()) + " " +
           toStr(vertexB.getZ()) + " " + toStr(vertexC.getX()) + " " +
           toStr(vertexC.getY()) + " " + toStr(vertexC.getZ()) + " mm";
  edgeCA = toStr(vertexC.getX()) + " " + toStr(vertexC.getY()) + " " +
           toStr(vertexC.getZ()) + " " + toStr(vertexA.getX()) + " " +
           toStr(vertexA.getY()) + " " + toStr(vertexA.getZ()) + " mm";
  edgeAMid = toStr(vertexA.getX()) + " " + toStr(vertexA.getY()) + " " +
             toStr(vertexA.getZ()) + " " + toStr(vertexMid.getX()) + " " +
             toStr(vertexMid.getY()) + " " + toStr(vertexMid.getZ()) + " mm";
  return {edgeAB, edgeBC, edgeCA, edgeAMid};
}

void Surface::FacetStore::DrawFacets() {
  G4UImanager *UI = G4UImanager::GetUIpointer();
  UI->ApplyCommand("/vis/verbose error");
  UI->ApplyCommand("/vis/set/colour 1 0 0");
  UI->ApplyCommand("/vis/set/linewidth 1.5");

  for (const auto &facet : fFacetVector) {
    FacetEdges edges{GetFacetLines(*facet)};
    UI->ApplyCommand("/vis/scene/add/line " + edges.edgeAB);
    UI->ApplyCommand("/vis/scene/add/line " + edges.edgeBC);
    UI->ApplyCommand("/vis/scene/add/line " + edges.edgeCA);
    UI->ApplyCommand("/vis/set/colour 0 1 1");
    UI->ApplyCommand("/vis/scene/add/line " + edges.edgeAMid);
    UI->ApplyCommand("/vis/set/colour 1 0 0");
  }

  UI->ApplyCommand("/vis/set/lineWidth");
  UI->ApplyCommand("/vis/set/colour");
  UI->ApplyCommand("/vis/verbose");
}

void Surface::FacetStore::LogFacetStore(const G4String &&aFilename) const {
  LogFacetStore(aFilename);
}

void Surface::FacetStore::AppendToFacetVector(G4TriangularFacet *aFacet) {
  fFacetVector.push_back(aFacet);
}

void Surface::FacetStore::LogFacetStore(const G4String &aFilename) const {
  if (aFilename == "") {
    return;
  }
  std::fstream out;
  out.open(aFilename, std::ios_base::app);
  if (!out.is_open()) {
    G4cout << "File not open" << G4endl;
    return;
  }
  for (size_t i = 0; i < fFacetVector.size(); ++i) {
    auto *face = fFacetVector.at(i);
    out << std::fixed << std::setprecision(10) << face->GetVertex(0) << " , ";
    out << std::fixed << std::setprecision(10) << face->GetVertex(1) << " , ";
    out << std::fixed << std::setprecision(10) << face->GetVertex(2) << " , ";
    out << std::fixed << std::setprecision(10) << fFacetProbability.at(i)
        << "\n";
  }
  out.close();
}

std::stringstream Surface::FacetStore::StreamInfo() const {
  std::stringstream ss;
  ss << "\n";
  ss << "\n";
  ss << "**************************************************\n";
  ss << "*         Information FacetStoreSampler          *\n";
  ss << "**************************************************\n";
  ss << "\n";
  ss << "Name: " << fName << "\n";
  ss << "Number of Facets: " << fFacetVector.size() << "\n";

  if (fLogger.WriteDetailInfo()) {
    ss << "\n";
    if (fLogger.WriteDebugInfo()) {
      ss << "Idx, P1,P2,P3,Area[mm^2],Probability %\n";
    } else {
      ss << "Idx, Area[mm^2],Probability %\n";
    }
    ss << "\n";
    G4double previousProbability{0};
    for (size_t i = 0; i < fFacetVector.size(); ++i) {
      const auto *facet = fFacetVector.at(i);
      ss << std::setw(5) << i << ",";
      if (fLogger.WriteDebugInfo()) {
        ss << std::fixed << std::setprecision(10) << facet->GetVertex(0) << ",";
        ss << std::fixed << std::setprecision(10) << facet->GetVertex(1) << ",";
        ss << std::fixed << std::setprecision(10) << facet->GetVertex(2) << ",";
      }
      ss << std::fixed << std::setprecision(10)
         << facet->GetArea() / (CLHEP::mm * CLHEP::mm) << ",";
      ss << std::fixed << std::setprecision(10)
         << (fFacetProbability.at(i) - previousProbability) * 100. << "\n";
      previousProbability = fFacetProbability.at(i);
    }

    ss << "\n";
    ss << "**************************************************\n";
  }
  return ss;
}

void Surface::FacetStore::PrintInfo() const {
  G4cout << StreamInfo().str() << G4endl;
}

std::vector<G4TriangularFacet *>::const_iterator
Surface::FacetStore::GetIterBegin() const {
  return fFacetVector.cbegin();
}

std::vector<G4TriangularFacet *>::const_iterator
Surface::FacetStore::GetIterEnd() const {
  return fFacetVector.cend();
}
