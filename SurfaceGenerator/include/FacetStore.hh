//
//
//
//
//      Author: cgruner
//

#ifndef SURFACE_FACETSTORE
#define SURFACE_FACETSTORE

#include <G4ThreeVector.hh>
#include <G4TriangularFacet.hh>
#include <iterator>
#include <vector>

namespace Surface {

/**
 *Holds a Store for Facets. All Facets together represent the surface of a
 *geometry placed in the global volume. It is possible to choose random points
 *on this surface which are normally distributed.
 */
class FacetStore {
private:
  struct FacetEdges {
    G4String edgeAB, edgeBC, edgeCA, edgeAMid;
  };

public:
  void CloseFacetStore();
  G4ThreeVector GetRandomPoint() const;
  void AppendToFacetVector(G4TriangularFacet *);
  void DrawFacets();
  inline G4bool GetIsStoreClosed() const { return fClosed; };
  void LogFacetStore(G4String &aFilename) const;
  void LogFacetStore(G4String &&aFilename) const;
  std::vector<G4TriangularFacet *>::const_iterator GetIterBegin() const;
  std::vector<G4TriangularFacet *>::const_iterator GetIterEnd() const;

private:
  void CalculateFacetProbability();
  FacetEdges GetFacetLines(const G4TriangularFacet &);
  std::vector<G4TriangularFacet *> fFacetVector;
  std::vector<G4double> fFacetProbability;
  G4bool fClosed{false};
};
} // namespace Surface
#endif // SURFACE_FACETSTORE
