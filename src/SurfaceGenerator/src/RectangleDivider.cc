/**
 * @brief definition class RectangleDivider
 * @file RectangleDivider.cc
 * @date 2023-05-14
 * @file C.Gruener
 */

#include "SurfaceGenerator/include/RectangleDivider.hh"
#include "G4Types.hh"
#include "G4ios.hh"
#include "Randomize.hh"

Surface::RectangleDivider::RectangleDivider(const G4double minSize,
                                            const G4double minX,
                                            const G4double maxX,
                                            const G4double minY,
                                            const G4double maxY)
    : fMinSize(minSize) {
  fRectangle.emplace_back(Rectangle{minX, maxX, minY, maxY});
  fFraction.emplace_back(1.);
}

/**
 * @brief Splits a rectangle randomly
 */
void Surface::RectangleDivider::SplitRectangleUniform() {
  const auto size = static_cast<double>(fRectangle.size());
  const auto randomRectangle =
      static_cast<G4int>(G4UniformRand() * size); // select random rectangle
  const G4bool direction = G4UniformRand() > 0.5 ? true : false;
  G4int counter{0};
  G4double split{G4UniformRand()};
  while (!SplitRectangleAt(randomRectangle, split, direction)) {
    split = G4UniformRand();
    ++counter;
    if (counter > 100) {
      G4cout << " **** Was not able to split rectangle " << G4endl;
      return;
    }
  }
}

/**
 * @brief Splits a selected rectangle
 * @param aRectangle id of rectangle to split
 * @param aSplit position of split
 * @param aDirection direction of split
 * @return true if split was successful, else false
 */
bool Surface::RectangleDivider::SplitRectangleAt(const G4int aRectangle,
                                                 const G4double aSplit,
                                                 const G4bool aDirection) {
  // Split one of the stored rectangles in two
  // Split Dimension, false splits in X direction, true in Y direction
  const Rectangle SelectedRec = fRectangle[aRectangle];
  const G4double SelectedFrac = fFraction[aRectangle];
  if (aDirection) {
    // split Y direction
    G4double diffY = (SelectedRec.maxY - SelectedRec.minY);
    if (diffY * aSplit < fMinSize || diffY * (1. - aSplit) < fMinSize) {
      return false;
    }
    diffY *= aSplit;
    fRectangle.emplace_back(Rectangle{SelectedRec.minX, SelectedRec.maxX,
                                      SelectedRec.minY + diffY,
                                      SelectedRec.maxY});
    fRectangle[aRectangle].maxY = SelectedRec.minY + diffY;
  } else {
    // split X direction
    G4double diffX = (SelectedRec.maxX - SelectedRec.minX);
    if (diffX * aSplit < fMinSize || diffX * (1. - aSplit) < fMinSize) {
      return false;
    }
    diffX *= aSplit;
    fRectangle.emplace_back(Rectangle{SelectedRec.minX + diffX,
                                      SelectedRec.maxX, SelectedRec.minY,
                                      SelectedRec.maxY});
    fRectangle[aRectangle].maxX = SelectedRec.minX + diffX;
  }
  fFraction.emplace_back(SelectedFrac * (1. - aSplit));
  fFraction[aRectangle] *= aSplit;
  return true;
}

void Surface::RectangleDivider::SplitRectangleEven(const G4int argX,
                                                   const G4int argY) {
  const Rectangle bigRectangle = fRectangle[0];
  fRectangle.pop_back();
  fFraction.pop_back();
  const G4double lenX = (bigRectangle.maxX - bigRectangle.minX) / argX;
  const G4double lenY = (bigRectangle.maxY - bigRectangle.minY) / argY;
  for (G4int x = 0; x < argX; ++x) {
    for (G4int y = 0; y < argY; ++y) {
      const G4double minX = bigRectangle.minX + x * lenX;
      const G4double maxX = bigRectangle.minX + (x + 1) * lenX;
      const G4double minY = bigRectangle.minY + y * lenX;
      const G4double maxY = bigRectangle.minY + (y + 1) * lenY;
      fRectangle.emplace_back(Rectangle{minX, maxX, minY, maxY});
      fFraction.push_back(1. / (argX * argY));
    }
  }
}

void Surface::RectangleDivider::SplitRectangle(const G4int SplitNRounds) {
  for (G4int i = 0; i < SplitNRounds; ++i) {
    SplitRectangleUniform();
  }
  }

std::vector<Surface::RectangleDivider::Rectangle>::iterator
Surface::RectangleDivider::GetIterBegin() {
  return fRectangle.begin();
}

std::vector<Surface::RectangleDivider::Rectangle>::iterator
Surface::RectangleDivider::GetIterEnd() {
  return fRectangle.end();
}

G4int Surface::RectangleDivider::GetNumberOfRectangles() const {
  return static_cast<G4int>(fRectangle.size());
}
