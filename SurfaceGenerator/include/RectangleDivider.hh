/*
 * RectangleDivider.hh
 *
 *  Created on: May 14, 2023
 *      Author: cgruner
 */

#ifndef INCLUDE_RECTANGLEDIVIDER_HH_
#define INCLUDE_RECTANGLEDIVIDER_HH_

#include "G4String.hh"
#include <vector>

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
  RectangleDivider(G4double, G4double, G4double, G4double, G4double);
  void SplitRectangleUniform();
  void SplitRectangleGaussian();
  void SplitRectangle(G4int);
  void SplitRectangleEven(G4int, G4int);
  std::vector<Rectangle>::iterator GetIterBegin();
  std::vector<Rectangle>::iterator GetIterEnd();
  G4int GetNumberOfRectangles();

private:
  G4bool SplitRectangleAt(G4int, G4double, G4bool);
  void SplitRectangleInHalf();
  G4double fminSize;
  std::vector<Rectangle> fRectangle;
  std::vector<G4double> fFraction;
};
} // namespace surface

#endif /* INCLUDE_RECTANGLEDIVIDER_HH_ */
