//
//
//
//
//      Author: cgruner
//

#ifndef SURFACE_FACETSTORE
#define SURFACE_FACETSTORE

#include <G4ThreeVector.hh>
#include <G4Transform3D.hh>
#include <G4TriangularFacet.hh>
#include <vector>

namespace Surface {

///
/// Stores G4Facets related to a surface. All Facets together represent the
/// surface of a geometry placed in the global volume. It is possible to choose
/// random points on this surface which are normally distributed.
///
class FacetStore {
private:
  struct FacetEdges {
    G4String edgeAB, edgeBC, edgeCA; ///< edge of a Triangular Facet.
    G4String edgeAMid; ///< edge splitting a Triangular Facet in half (VertexA
                       ///< <--> Mid(edgeBC)).
  };

public:
  ///
  /// Closes Facet Store and prepares it for usage in simulation.
  ///
  void CloseFacetStore();
  ///
  /// Returns random point of facet.
  ///
  G4ThreeVector GetRandomPoint() const;
  ///
  /// Appends Triangular Facet to Fact Store.
  /// \param facet Pointer to facet which will be added to store
  ///
  void AppendToFacetVector(G4TriangularFacet *facet);
  ///
  /// Draws all Facets in FacetsStore using the G4UImanager.
  ///
  void DrawFacets();
  ///
  /// Indicates if store is closed.
  ///
  inline G4bool GetIsStoreClosed() const { return fClosed; };
  ///
  /// Writes Vertices of Facet and its share of surface area to file.
  ///\param aFilename Name of logfile
  ///
  void LogFacetStore(const G4String &aFilename) const;
  void LogFacetStore(const G4String &&aFilename) const;
  std::vector<G4TriangularFacet *>::const_iterator GetIterBegin() const;
  std::vector<G4TriangularFacet *>::const_iterator GetIterEnd() const;
  void SetTransformation(const G4ThreeVector &transformation) {
    fTransform = transformation;
  };

private:
  ///
  /// Fills the fFacetProbability variable.
  /// Calculates the shares of the area of the single triangular facets to the
  /// total area of all triangular facets. Results are stored in the
  /// fFacetProbability vector.
  ///
  void CalculateFacetProbability();
  ///
  /// Returns edges of selected facet
  /// \param facet Facet to get edges from
  ///
  FacetEdges GetFacetLines(const G4TriangularFacet &facet) const;
  std::vector<G4TriangularFacet *>
      fFacetVector; ///< vector of Triangular Facets
  std::vector<G4double>
      fFacetProbability; ///< Stores share of single Triangular Facet area to
                         ///< total area.
  G4bool fClosed{false}; ///< Indicates if Facet Store is closed and facets can
                         ///< not be added anymore.
  G4ThreeVector fTransform;
};
} // namespace Surface
#endif // SURFACE_FACETSTORE
