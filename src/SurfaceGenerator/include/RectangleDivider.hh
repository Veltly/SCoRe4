/**
 * @brief Describes the class RectangleDivider
 * @file RectangleDivider.hh
 * @date 2023-05-14
 * @author C.Gruener
 */
#ifndef SRC_SURFACE_GENERATOR_INCLUDE_RECTANGLE_DIVIDER_HH_
#define SRC_SURFACE_GENERATOR_INCLUDE_RECTANGLE_DIVIDER_HH_

#include <vector>
#include "G4Types.hh"

namespace Surface {

/**
 * @class RectangleDivider
 * @brief Class to split a rectangle in smaller rectangles adn return their boundaries
 */
class RectangleDivider {
 public:
  /**
   * @struct Rectangle
   * @brief definition of a rectangle
   */
  struct Rectangle {
    G4double minX, maxX, minY, maxY;
  };

 public:
  /**
   * @brief Constructor of a rectangle
   */
  RectangleDivider(G4double, G4double, G4double,
                   G4double, G4double);
  void SplitRectangleUniform();
  void SplitRectangle(G4int);
  void SplitRectangleEven(G4int, G4int);
  std::vector<Rectangle>::iterator GetIterBegin();
  std::vector<Rectangle>::iterator GetIterEnd();
  G4int GetNumberOfRectangles() const;

 private:
  G4bool SplitRectangleAt(G4int, G4double, G4bool);
  G4double fMinSize;
  std::vector<Rectangle> fRectangle;
  std::vector<G4double> fFraction;
};
}
#endif