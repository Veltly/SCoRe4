/**
 * @brief Functions to calculate surface parameters
 * @author C.Gruener
 * @date 2023-06-01
 * @file Calculator.cc
 */

#include "SurfaceGenerator/include/Calculator.hh"

#include <algorithm>
#include <numeric>

#include "CLHEP/Units/SystemOfUnits.h"
#include "G4ThreeVector.hh"

Surface::Calculator::Calculator(Surface::FacetStore *aFacetStore)
    : fFacetStore(aFacetStore) {
  Recalculate();
}

void Surface::Calculator::Recalculate() {
  MeanHeight = 0;
  ProjectedSurface = CalcProjectedSurface();
  MeanHeight = CalcMeanHeight();
  Sv = CalcSv();
  Sp = CalcSp();
  Sz = CalcSz();
  Sq = CalcSq();
  Sa = CalcSa();
  Sku = CalcSku();
  Ssk = CalcSsk();
  area = CalcArea();
}

G4double Surface::Calculator::Integrate_f_0() { return 0.5; }

G4double Surface::Calculator::Integrate_f_1(
    const FunctionParameter &aParameter) {
  const G4double a = aParameter.a;
  const G4double b = aParameter.b;
  const G4double c = aParameter.c;
  const G4double Integral = a / 2. + b / 6. + c / 6.;
  return Integral;
}

G4double Surface::Calculator::Integrate_f_2(
    const FunctionParameter &aParameter) {
  const G4double a = aParameter.a;
  const G4double b = aParameter.b;
  const G4double c = aParameter.c;
  G4double tmp[6];
  tmp[0] = std::pow(a, 2) / 2.;
  tmp[1] = a * b / 3.;
  tmp[2] = a * c / 3.;
  tmp[3] = std::pow(b, 2) / 12.;
  tmp[4] = b * c / 12.;
  tmp[5] = std::pow(c, 2) / 12.;
  const G4int length = sizeof(tmp) / sizeof(tmp[0]);
  const G4double Integral = std::accumulate(tmp, tmp + length, 0.);
  return Integral;
}

G4double Surface::Calculator::Integrate_f_3(
    const FunctionParameter &aParameter) {
  const G4double a = aParameter.a;
  const G4double b = aParameter.b;
  const G4double c = aParameter.c;
  G4double tmp[10];
  tmp[0] = std::pow(a, 3) / 2.;
  tmp[1] = std::pow(a, 2) * b / 2.;
  tmp[2] = std::pow(a, 2) * c / 2.;
  tmp[3] = a * std::pow(b, 2) / 4.;
  tmp[4] = a * b * c / 4.;
  tmp[5] = a * std::pow(c, 2) / 4.;
  tmp[6] = std::pow(b, 3) / 20.;
  tmp[7] = std::pow(b, 2) * c / 20.;
  tmp[8] = b * std::pow(c, 2) / 20.;
  tmp[9] = std::pow(c, 3) / 20.;
  const G4int length = sizeof(tmp) / sizeof(tmp[0]);
  const G4double Integral = std::accumulate(tmp, tmp + length, 0.);
  return Integral;
}

G4double Surface::Calculator::Integrate_f_4(
    const FunctionParameter &aParameter) {
  const G4double a = aParameter.a;
  const G4double b = aParameter.b;
  const G4double c = aParameter.c;
  G4double tmp[15];
  tmp[0] = std::pow(a, 4) / 2.;
  tmp[1] = 2 * std::pow(a, 3) * b / 3.;
  tmp[2] = 2 * std::pow(a, 3) * c / 3.;
  tmp[3] = std::pow(a, 2) * std::pow(b, 2) / 2.;
  tmp[4] = std::pow(a, 2) * b * c / 2.;
  tmp[5] = std::pow(a, 2) * std::pow(c, 2) / 2.;
  tmp[6] = a * std::pow(b, 3) / 5.;
  tmp[7] = a * std::pow(b, 2) * c / 5.;
  tmp[8] = a * b * std::pow(c, 2) / 5.;
  tmp[9] = a * std::pow(c, 3) / 5.;
  tmp[10] = std::pow(b, 4) / 30.;
  tmp[11] = std::pow(b, 3) * c / 30.;
  tmp[12] = std::pow(b, 2) * std::pow(c, 2) / 30.;
  tmp[13] = b * std::pow(c, 3) / 30.;
  tmp[14] = std::pow(c, 4) / 30.;
  const G4int length = sizeof(tmp) / sizeof(tmp[0]);
  const G4double Integral = std::accumulate(tmp, tmp + length, 0.);
  return Integral;
}

Surface::Calculator::Vertices Surface::Calculator::GetVertices(
    const G4TriangularFacet &aFacet) {
  return {aFacet.GetVertex(0), aFacet.GetVertex(1), aFacet.GetVertex(2)};
}

void Surface::Calculator::OrderVertices(Vertices &aVertices) {
  G4ThreeVector &vertex1 = aVertices.p1;
  G4ThreeVector &vertex2 = aVertices.p2;
  G4ThreeVector &vertex3 = aVertices.p3;
  if (vertex2.x() < vertex1.x()) {
    std::swap(vertex1, vertex2);
  }
  if (vertex3.x() < vertex1.x()) {
    std::swap(vertex1, vertex3);
  }
  if (vertex2.x() < vertex3.x()) {
    std::swap(vertex2, vertex3);
  }
}

void Surface::Calculator::ShiftToZero(Vertices &aVertices) {
  const G4double ShiftX = -aVertices.p1.x();
  const G4double ShiftY = -aVertices.p1.y();
  const G4ThreeVector Shift{ShiftX, ShiftY, 0};
  aVertices.p1 += Shift;
  aVertices.p2 += Shift;
  aVertices.p3 += Shift;
}

void Surface::Calculator::ShiftToMean(Vertices &aVertices) const {
  G4ThreeVector Shift{0, 0, MeanHeight};
  aVertices.p1 -= Shift;
  aVertices.p2 -= Shift;
  aVertices.p3 -= Shift;
}

Surface::Calculator::FunctionParameter
Surface::Calculator::GetFunctionParameter(const Vertices &aVertices) {
  const G4double ParameterA{aVertices.p1.z()};
  const G4double tmp1 = aVertices.p3.z() * aVertices.p2.x();
  const G4double tmp2 = aVertices.p3.x() * aVertices.p2.z();
  const G4double tmp3 = aVertices.p1.z() * aVertices.p2.x();
  const G4double tmp4 = aVertices.p1.z() * aVertices.p3.x();
  const G4double tmp5 = aVertices.p2.x() * aVertices.p3.y();
  const G4double tmp6 = aVertices.p3.x() * aVertices.p2.y();
  const G4double ParameterC = (tmp1 - tmp2 - tmp3 + tmp4) / (tmp5 - tmp6);
  const G4double tmp7 = ParameterC * aVertices.p2.y();
  const G4double ParameterB =
      (aVertices.p2.z() - ParameterA - tmp7) / aVertices.p2.x();

  return {ParameterA, ParameterB, ParameterC};
}

G4double Surface::Calculator::GetTrafoDeterminant(const Vertices &aVertices) {
  const G4double tmp1 = aVertices.p2.x() * aVertices.p3.y();
  const G4double tmp2 = aVertices.p2.y() * aVertices.p3.x();
  return std::abs(tmp1 - tmp2);
}

G4double Surface::Calculator::CalcProjectedSurface() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double Surface{0};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet =
        *(*FacetIter);  // Double dereferencing because ITER to FACET-POINTER
                        // to FACET;
    auto vertices = GetVertices(facet);
    OrderVertices(vertices);
    ShiftToZero(vertices);
    Surface += GetTrafoDeterminant(vertices) * Integrate_f_0();
    ++FacetIter;
  }
  return Surface;
}

G4double Surface::Calculator::CalcMeanHeight() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double Volume{0};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet = *(*FacetIter);
    auto vertices = GetVertices(facet);
    Volume += IntegrationRoutine(vertices, Integrate_f_1);
    ++FacetIter;
  }
  return Volume / ProjectedSurface;
}

G4double Surface::Calculator::CalcSa() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double Value{0};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet =
        *(*FacetIter);  // Double dereferencing because ITER to FACET-POINTER
                        // to FACET;
    auto vertices = GetVertices(facet);
    if (IsIntersected(vertices)) {
      auto newVertices = Split(vertices);
      Value += std::abs(IntegrationRoutine(newVertices.at(0), Integrate_f_1));
      Value += std::abs(IntegrationRoutine(newVertices.at(1), Integrate_f_1));
      Value += std::abs(IntegrationRoutine(newVertices.at(2), Integrate_f_1));
    } else {
      Value += std::abs(IntegrationRoutine(vertices, Integrate_f_1));
    }
    ++FacetIter;
  }
  return Value / ProjectedSurface;
}

G4double Surface::Calculator::CalcSz() { return CalcSp() + CalcSv(); }

G4double Surface::Calculator::CalcSv() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double min{MeanHeight};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet =
        *(*FacetIter);  // Double dereferencing because ITER to FACET-POINTER
                        // to FACET;
    auto vertices = GetVertices(facet);
    G4ThreeVector LowestVertex = GetLowestVertex(vertices);
    if (LowestVertex.z() < min) {
      min = LowestVertex.z();
    }
    ++FacetIter;
  }
  return MeanHeight - min;
}

G4double Surface::Calculator::CalcSp() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double max{MeanHeight};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet =
        *(*FacetIter);  // Double dereferencing because ITER to FACET-POINTER
                        // to FACET;
    auto vertices = GetVertices(facet);
    G4ThreeVector HighestVertex = GetHighestVertex(vertices);
    if (HighestVertex.z() > max) {
      max = HighestVertex.z();
    }
    ++FacetIter;
  }
  return max - MeanHeight;
}

G4double Surface::Calculator::CalcSq() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double Value{0};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet =
        *(*FacetIter);  // Double dereferencing because ITER to FACET-POINTER
                        // to FACET;
    auto vertices = GetVertices(facet);
    Value += IntegrationRoutine(vertices, Integrate_f_2);
    ++FacetIter;
  }
  return std::sqrt(Value / ProjectedSurface);
}

G4double Surface::Calculator::CalcSsk() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double Value{0};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet =
        *(*FacetIter);  // Double dereferencing because ITER to FACET-POINTER
                        // to FACET;
    auto vertices = GetVertices(facet);
    Value += IntegrationRoutine(vertices, Integrate_f_3);
    ++FacetIter;
  }
  Value /= ProjectedSurface;
  Value /= std::pow(Sq, 3);
  return Value;
}

G4double Surface::Calculator::CalcSku() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double Value{0};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet =
        *(*FacetIter);  // Double dereferencing because ITER to FACET-POINTER
                        // to FACET;
    auto vertices = GetVertices(facet);
    Value += IntegrationRoutine(vertices, Integrate_f_4);
    ++FacetIter;
  }
  Value /= ProjectedSurface;
  Value /= std::pow(Sq, 4);
  return Value;
}

G4double Surface::Calculator::CalcArea() {
  auto FacetIter = fFacetStore->GetIterBegin();
  auto FacetIterEnd = fFacetStore->GetIterEnd();
  G4double surface{0};
  while (FacetIter != FacetIterEnd) {
    const G4TriangularFacet &facet =
        *(*FacetIter);  // Double dereferencing because ITER to FACET-POINTER
                        // to FACET;
    surface += facet.GetArea();
    ++FacetIter;
  }
  return surface;
}

G4double Surface::Calculator::IntegrationRoutine(
    Vertices &aVertices, G4double (*aIntegration)(const FunctionParameter &)) {
  OrderVertices(aVertices);
  ShiftToZero(aVertices);
  ShiftToMean(aVertices);
  FunctionParameter parameter = GetFunctionParameter(aVertices);
  TransformFunctionParameter(parameter, aVertices);
  return GetTrafoDeterminant(aVertices) * aIntegration(parameter);
}

G4bool Surface::Calculator::IsIntersected(const Vertices &aVertices) const {
  if (MeanHeight <= aVertices.p1.z() && MeanHeight <= aVertices.p2.z() &&
      MeanHeight <= aVertices.p3.z()) {
    return false;
  }
  if (MeanHeight >= aVertices.p1.z() && MeanHeight >= aVertices.p2.z() &&
      MeanHeight >= aVertices.p3.z()) {
    return false;
  }
  return true;
}

std::array<Surface::Calculator::Vertices, 3> Surface::Calculator::Split(
    Vertices &aVertices) {
  MoveSinglePointToP1(aVertices);
  const G4ThreeVector IntersectionAB = SplitEdge(aVertices.p1, aVertices.p2);
  const G4ThreeVector IntersectionAC = SplitEdge(aVertices.p1, aVertices.p3);
  const Vertices VerticesA{aVertices.p1, IntersectionAB, IntersectionAC};
  const Vertices VerticesB{IntersectionAB, aVertices.p2, aVertices.p3};
  const Vertices VerticesC{IntersectionAB, IntersectionAC, aVertices.p3};
  return std::array<Surface::Calculator::Vertices, 3>{VerticesA, VerticesB,
                                                      VerticesC};
}

G4ThreeVector Surface::Calculator::SplitEdge(const G4ThreeVector &aPointA,
                                             const G4ThreeVector &aPointB) const {
  G4double factor = MeanHeight - aPointA.z();
  factor /= aPointB.z() - aPointA.z();
  const G4ThreeVector delta = aPointB - aPointA;
  return aPointA + factor * delta;
}

void Surface::Calculator::MoveSinglePointToP1(Vertices &aVertices) const {
  if (aVertices.p1.z() > MeanHeight && aVertices.p2.z() > MeanHeight &&
      aVertices.p3.z() <= MeanHeight) {
    std::swap(aVertices.p1, aVertices.p3);  // P3 single and lowest point
  } else if (aVertices.p1.z() > MeanHeight && aVertices.p2.z() <= MeanHeight &&
             aVertices.p3.z() > MeanHeight) {
    std::swap(aVertices.p1, aVertices.p2);  // P2 single and lowest point
  } else if (aVertices.p1.z() < MeanHeight && aVertices.p2.z() >= MeanHeight &&
             aVertices.p3.z() < MeanHeight) {
    std::swap(aVertices.p1, aVertices.p2);  // P2 single and highest point
  } else if (aVertices.p1.z() < MeanHeight && aVertices.p2.z() < MeanHeight &&
             aVertices.p3.z() >= MeanHeight) {
    std::swap(aVertices.p1, aVertices.p3);  // P3 single and highest Point
  }
}

G4ThreeVector Surface::Calculator::GetLowestVertex(const Vertices &aVertices) {
  G4ThreeVector LowestVertex = aVertices.p1;
  if (aVertices.p2.z() < LowestVertex.z()) {
    LowestVertex = aVertices.p2;
  }
  if (aVertices.p3.z() < LowestVertex.z()) {
    LowestVertex = aVertices.p3;
  }
  return LowestVertex;
}

G4ThreeVector Surface::Calculator::GetHighestVertex(const Vertices &aVertices) {
  G4ThreeVector HighestVertex = aVertices.p1;
  if (aVertices.p2.z() > HighestVertex.z()) {
    HighestVertex = aVertices.p2;
  }
  if (aVertices.p3.z() < HighestVertex.z()) {
    HighestVertex = aVertices.p3;
  }
  return HighestVertex;
}

void Surface::Calculator::PrintSurfaceInformation() const {
  G4cout << StreamSurfaceInformation().str() << G4endl;
}

std::stringstream Surface::Calculator::StreamSurfaceInformation() const {
  std::stringstream ss;
  ss << G4endl;
  ss << "**************************************************\n";
  ss << "*              Surface  Information              *\n";
  ss << "**************************************************\n";
  ss << "FacetStore name: " << fFacetStore->GetStoreName() << "\n";
  ss << "Covered Surface: " << GetProjectedSurface() / (CLHEP::mm * CLHEP::mm)
     << " mm^2\n";
  ss << "Mean height    : " << GetMeanHeight() / CLHEP::mm << " mm\n";
  ss << "Sv             : " << GetSv() / CLHEP::mm << " mm\n";
  ss << "Sp             : " << GetSp() / CLHEP::mm << " mm\n";
  ss << "Sz             : " << GetSz() / CLHEP::mm << " mm\n";
  ss << "Sq             : " << GetSq() / CLHEP::mm << " mm\n";
  ss << "Sa             : " << GetSa() / CLHEP::mm << " mm\n";
  ss << "Ssk            : " << GetSsk() << "\n";
  ss << "Sku            : " << GetSku() << "\n";
  ss << "Area           : " << GetArea() / (CLHEP::mm * CLHEP::mm) << " mm^2\n";
  ss << "Nr of elements : " << fFacetStore->Size() << "\n";
  ss << "\n";
  ss << "**************************************************\n";
  return ss;
}

void Surface::Calculator::TransformFunctionParameter(
    FunctionParameter &aFuncParameter, const Vertices &aVertices) {
  const G4double b = aFuncParameter.b;
  const G4double c = aFuncParameter.c;
  aFuncParameter.b = b * aVertices.p2.x() + c * aVertices.p2.y();
  aFuncParameter.c = b * aVertices.p3.x() + c * aVertices.p3.y();
}
