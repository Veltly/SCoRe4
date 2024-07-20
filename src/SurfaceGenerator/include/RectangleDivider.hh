// Copyright [2024] C.Gruener
// Date: 23-05-14
// File:

#ifndef SRC_SURFACEGENERATOR_INCLUDE_RECTANGLEDIVIDER_HH_
#define SRC_SURFACEGENERATOR_INCLUDE_RECTANGLEDIVIDER_HH_

#include <vector>

#include "G4Types.hh"

namespace Surface {

/**
 * Class to split a Rectangle in smaller rectangles
 */

class RectangleDivider {
 public:
  struct Rectangle {
    G4double minX, maxX, minY, maxY;
  };

 public:
  RectangleDivider(const G4double, const G4double, const G4double,
                   const G4double, const G4double);
  void SplitRectangleUniform();
  void SplitRectangleGaussian();
  void SplitRectangle(const G4int);
  void SplitRectangleEven(const G4int, const G4int);
  std::vector<Rectangle>::iterator GetIterBegin();
  std::vector<Rectangle>::iterator GetIterEnd();
  G4int GetNumberOfRectangles() const;

 private:
  G4bool SplitRectangleAt(const G4int, const G4double, const G4bool);
  void SplitRectangleInHalf();
  G4double fminSize;
  std::vector<Rectangle> fRectangle;
  std::vector<G4double> fFraction;
};
}  // namespace Surface
#endif  // SRC_SURFACEGENERATOR_INCLUDE_RECTANGLEDIVIDER_HH_
