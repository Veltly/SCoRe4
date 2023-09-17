//
//
//
//
//	author: C.Gruener
//
//

#ifndef SURFACE_CALCULATOR
#define SURFACE_CALCULATOR

#include "FacetStore.hh"
#include <G4ThreeVector.hh>

namespace Surface {

class Calculator {
  struct Vertices {
    G4ThreeVector p1, p2, p3;
  };
  struct FunctionParameter {
    G4double a, b, c;
  };

public:
  Calculator(Surface::FacetStore &);
  void Recalculate();

private:
  G4double CalcProjectedSurface();
  G4double CalcMeanHeight();
  G4double CalcSa();
  G4double CalcSz();
  G4double CalcSv();
  G4double CalcSp();
  G4double CalcSku();
  G4double CalcSsk();
  G4double CalcSq();

  static Vertices GetVertices(const G4TriangularFacet &aFacet);
  static void OrderVertices(Vertices &aVertices);
  static void ShiftToZero(Vertices &aVertices);
  void ShiftToMean(Vertices &aVertices);
  static FunctionParameter GetFunctionParameter(const Vertices &aVertices);
  static G4double GetTrafoDeterminant(const Vertices &aVertices);
 
  G4double
  IntegrationRoutine(Vertices &aVertices,
                     G4double (*aIntegration)(const FunctionParameter &));

  static G4double Integrate_f_0();
  static G4double Integrate_f_1(const FunctionParameter &aParameter);
  static G4double Integrate_f_2(const FunctionParameter &aParameter);
  static G4double Integrate_f_3(const FunctionParameter &aParameter);
  static G4double Integrate_f_4(const FunctionParameter &aParameter);
  G4bool IsIntersected(Vertices&);
  std::array<Vertices,3> Split(Vertices&);
  G4ThreeVector SplitEdge(const G4ThreeVector&, const G4ThreeVector&);
  void MoveSinglePointToP1(Vertices&);
  G4ThreeVector GetLowestVertex(const Vertices&);
  G4ThreeVector GetHighestVertex(const Vertices&);
  Surface::FacetStore fFacetStore;
  G4double MeanHeight;
  G4double ProjectedSurface;
};
} // namespace Surface
#endif
