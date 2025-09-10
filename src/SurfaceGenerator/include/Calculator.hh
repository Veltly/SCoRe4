/**
 * @brief Calculates surface parameters
 * @author C.Gruener
 * @date 2023-06-01
 * @file Calculator.hh
 */

#ifndef SRC_SURFACEGENERATOR_INCLUDE_CALCULATOR_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_CALCULATOR_HH_

#include "G4ThreeVector.hh"
#include "SurfaceGenerator/include/FacetStore.hh"

namespace Surface {
/**
 * @brief Calculates different parameters representing the surface, based on the FacetStore.
 */
class Calculator {
  struct Vertices {
    G4ThreeVector p1, p2, p3;
  };

  /**
   * @brief Function Parameters for Integrating functions. Parameter are the heights of vertices of triangle.
   * @param a : height of (0,0)
   * @param b : height of (1,0)
   * @param c : height of (0,1)
   */
  struct FunctionParameter {
    G4double a, b, c;
  };

 public:
  /**
   * @brief Instantiates calculator.
   * @param FacetStore has to be passed
   */
  explicit Calculator(Surface::FacetStore *);

  /**
   * @brief Recalculates all values.
   */
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
  inline G4double GetArea() const { return area; }

  /**
   * @brief Prints surface information to console
   */
  void PrintSurfaceInformation() const;

  /**
   * @brief Returns a stream object with surface information
   */
  std::stringstream StreamSurfaceInformation() const;

 private:
  /**
   * @brief Calculate projected surface.
   * @details \f$ A = \int \int_A dxdy \f$
   */
  G4double CalcProjectedSurface();
  /**
   * @brief Calculate mean height.
   * @details \f$ mean = \frac{1}{A} \int \int_A Z(x,y) dxdy \f$
   */
  G4double CalcMeanHeight();
  /**
   * @brief Calculate arithmetical mean height.
   * @details \f$ Sa = \frac{1}{A} \int \int_A |Z(x,y)| dxdy \f$
   */
  G4double CalcSa();
  /**
   * @brief Calculate maximal Height.
   * @details \f$ Sz = Sv + Sp \f$
   */
  G4double CalcSz();
  /**
   * @brief Calculate maximal pit height.
   * @details \f$ Sv = |min_A (mean - Z(x,y))| \f$
   */
  G4double CalcSv();
  /**
   * @brief Calculate maximal peak height.
   * @details \f$ Sp = max_A (Z(x,y) - mean) \f$
   */
  G4double CalcSp();
  /**
   * @brief Calculate kurtosis.
   * @details \f$ Sku = \frac{1}{Sq^4} \frac{1}{A} \int \int_A Z^4(x,y)dxdy \f$
   */
  G4double CalcSku();
  /**
   * @brief Calculate Skewness.
   * @details \f$ Ssk = \frac{1}{Sq^3} \frac{1}{A} \int \int_A Z^3(x,y)dxdy \f$
   */
  G4double CalcSsk();
  /**
   * @brief Calculate root mean square height.
   * @details \f$ Sq = \sqrt{\frac{1}{A} \int \int_A Z^2(x,y)dxdy} \f$
   */
  G4double CalcSq();
  /**
   * @brief Calculate area of surface
   */
  G4double CalcArea();
  /**
   * @brief Returns vertices from passed facet.
   * @details
   */
  static Vertices GetVertices(const G4TriangularFacet &aFacet);
  /**
   * @brief Orders vertices based on X-component.
   * @details At the End order will be: vertex1.x < vertex2.x < vertex3.x
   */
  static void OrderVertices(Vertices &aVertices);
  /**
   * @brief Will shift all vertices such that vertex1 will be at x = 0 and y = 0.
   */
  static void ShiftToZero(Vertices &aVertices);
  /**
   * @brief Shifts vertices by MeanHeight in -z direction
   */
  void ShiftToMean(Vertices &aVertices) const;
  /**
   * @brief Compute parameter used for integration out of vertices.
   * @details FunctionParameter represent 3 parameter.
   */
  static FunctionParameter GetFunctionParameter(const Vertices &aVertices);
  static G4double GetTrafoDeterminant(const Vertices &aVertices);

  /**
   * @brief Integration of a surface function over a triangle surface.
   * @details Integration is done over a triangle with points (x,y):
   * (0,0), (1,0), (0,1).
   * @param aVertices defines triangle.
   * @param aIntegration is pointer to integrating function.
   */
  G4double IntegrationRoutine(
      Vertices &aVertices, G4double (*aIntegration)(const FunctionParameter &));

  static G4double Integrate_f_0();
  static G4double Integrate_f_1(const FunctionParameter &aParameter);
  static G4double Integrate_f_2(const FunctionParameter &aParameter);
  static G4double Integrate_f_3(const FunctionParameter &aParameter);
  static G4double Integrate_f_4(const FunctionParameter &aParameter);

  G4bool IsIntersected(const Vertices &) const;

  std::array<Vertices, 3> Split(Vertices &);
  G4ThreeVector SplitEdge(const G4ThreeVector &, const G4ThreeVector &) const;
  void MoveSinglePointToP1(Vertices &) const;

  static G4ThreeVector GetLowestVertex(const Vertices &);
  static G4ThreeVector GetHighestVertex(const Vertices &);
  static void TransformFunctionParameter(FunctionParameter &aFuncParameter, const Vertices &aVertices);

 private:
  Surface::FacetStore *fFacetStore;
  G4double MeanHeight{};        ///< Mean height
  G4double ProjectedSurface{};  ///< Area of projected surface
  G4double Sz{};                ///< Maximum Height
  G4double Sa{};                ///< Arithmetical mean height
  G4double Sv{};                ///< Maximum pit height
  G4double Sp{};                ///< Maximum peak height
  G4double Sku{};               ///< Kurtosis
  G4double Ssk{};               ///< Skewness
  G4double Sq{};                ///< Root mean square height
  G4double area{};              ///< Area of surface
};
}  // namespace Surface
#endif
