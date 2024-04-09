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
  inline G4double GetProjectedSurface() const { return ProjectedSurface; }
  inline G4double GetMeanHeight() const { return MeanHeight; }
  inline G4double GetSz() const { return Sz; }
  inline G4double GetSa() const { return Sa; }
  inline G4double GetSv() const { return Sv; }
  inline G4double GetSp() const { return Sp; }
  inline G4double GetSku() const { return Sku; }
  inline G4double GetSsk() const { return Ssk; }
  inline G4double GetSq() const { return Sq; }

  void PrintSurfaceInformation();

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
  G4bool IsIntersected(Vertices &);
  std::array<Vertices, 3> Split(Vertices &);
  G4ThreeVector SplitEdge(const G4ThreeVector &, const G4ThreeVector &);
  void MoveSinglePointToP1(Vertices &);
  static G4ThreeVector GetLowestVertex(const Vertices &);
  static G4ThreeVector GetHighestVertex(const Vertices &);
  static void TransformFunctionparameter(FunctionParameter&, const Vertices&);
  Surface::FacetStore fFacetStore;
  G4double MeanHeight;
  G4double ProjectedSurface;
  G4double Sz;
  G4double Sa;
  G4double Sv;
  G4double Sp;
  G4double Sku;
  G4double Ssk;
  G4double Sq;
};
} // namespace Surface
#endif
