/**
 * @brief Stores and handles G4TriangularFacets
 * @author C.Gruener
 * @date 2023-06-01
 * @file FacetStore.hh
 */
#ifndef SRC_SURFACEGENERATOR_INCLUDE_FACETSTORE_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_FACETSTORE_HH_

#include <vector>

#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4TriangularFacet.hh"
#include "Service/include/Logger.hh"

namespace Surface {

/**
 * @brief Stores and handles G4TriangularFacets
 * @details Stores and handles G4TriangularFacets to represent a surface.
 * Allows further to sample uniformly distributed points on this surface and
 * to draw the surface using a GUI.
 */
class FacetStore {
 private:
  struct FacetEdges {
    G4String edgeAB, edgeBC, edgeCA;  /// edge of a Triangular Facet.
    G4String edgeAMid;  /// edge splitting a Triangular Facet in half (VertexA
                        /// <--> Mid(edgeBC)).
  };

 public:
  explicit FacetStore(const G4String &name , VerboseLevel verboseLvl = VerboseLevel::Default)
      : fName("FacetStore_" + name), fLogger("FacetStore_" + name, verboseLvl) {}

/**
 * @brief Closes FacetStore and prepares it for usage in simulation.
 * After closing it, no facets can be added anymore.
 */
  void CloseFacetStore();

  /**
   * @brief Returns a randomly sampled point from surface
   * @return
   */
  G4ThreeVector GetRandomPoint() const;
  /**
   * @brief Returns a randomly sampled point from surface and stores facet normal vector in argument
   */
  G4ThreeVector GetRandomPoint(G4ThreeVector &surfaceNormal);
  /**
   * @brief Appends Triangular Facet to Fact Store.
   * @param facet Pointer to facet which will be added to store
   */
  void AppendToFacetVector(G4TriangularFacet *facet);

  void DrawFacets();

  inline G4bool GetIsStoreClosed() const { return fClosed; }
  ///
  /// Writes Vertices of Facet and its share of surface area to file.
  /// \param aFilename Name of logfile
  ///
  void LogFacetStore(const G4String &aFilename) const;
  void LogFacetStore(const G4String &&aFilename) const;
  void PrintInfo() const;
  std::stringstream StreamInfo() const;

  std::vector<G4TriangularFacet *>::const_iterator GetIterBegin() const;
  std::vector<G4TriangularFacet *>::const_iterator GetIterEnd() const;
  void SetTransformation(const G4ThreeVector &transformation) {
    fTransform = transformation;
  }
  void SetTransformation(const G4Transform3D &trafo) {
    fTransform = trafo.getTranslation();
  }

  inline G4ThreeVector GetTransformation() const { return fTransform; }
  inline G4String GetStoreName() const { return fName; }

  inline G4int Size() const { return static_cast<G4int>(fFacetVector.size()); }

 private:
  /**
   * @brief Calculates the facets relative surface
   * @details Calculates the facets relative surface in comparison to total
   * surface of all facets in the facet store and stores result in fFacetProbability.
   * Sum of all values is equal to one.
   */
  void CalculateFacetProbability();
  /**
   * @brief returns edges of selected facet
   * @param facet Facet to get edges from
   * @return
   */
  static FacetEdges GetFacetLines(const G4TriangularFacet &facet) ;

  std::vector<G4TriangularFacet *>
      fFacetVector;  ///< vector of Triangular Facets
  std::vector<G4double>
      fFacetProbability;  ///< Stores share of single Triangular Facet area to
                          ///< total area.
  G4bool fClosed{false};  ///< Indicates if Facet Store is closed and facets can
                          ///< not be added anymore.
  G4ThreeVector fTransform; ///< Stores coordinates of FacetStore
  G4String fName;
  Surface::Logger fLogger;
};
}  // namespace Surface
#endif  // SRC_SURFACEGENERATOR_INCLUDE_FACETSTORE_HH_
