// Copyright [2024] C.Gruener
// Date: 23-05-14
// File:

#include "SurfaceGenerator/include/RectangleDivider.hh"

#include "G4Types.hh"
#include "G4ios.hh"
#include "Randomize.hh"

Surface::RectangleDivider::RectangleDivider(const G4double minSize,
                                            const G4double minX,
                                            const G4double maxX,
                                            const G4double minY,
                                            const G4double maxY)
    : fminSize(minSize) {
  fRectangle.emplace_back(Rectangle{minX, maxX, minY, maxY});
  fFraction.emplace_back(1.);
}

void Surface::RectangleDivider::SplitRectangleUniform() {
  G4int RandomRectangle =
      static_cast<G4int>(G4UniformRand() * fRectangle.size());

  G4double split = G4UniformRand();
  G4bool Direction{false};
  if (G4UniformRand() > 0.5) {
    Direction = true;
  }

  G4int counter{0};
  while (!SplitRectangleAt(RandomRectangle, split, Direction)) {
    split = G4UniformRand();
    ++counter;
    if (counter > 100) {
      G4cout << " **** Was not able to split rectangle " << G4endl;
      return;
    }
  }
}

// not finished
void Surface::RectangleDivider::SplitRectangleGaussian() {
  G4int RandomRectangle =
      static_cast<G4int>(G4UniformRand() * fRectangle.size());

  G4double split = G4UniformRand();
  G4bool Direction{false};
  if (G4UniformRand() > 0.5) {
    Direction = true;
  }

  G4int counter{0};
  while (!SplitRectangleAt(RandomRectangle, split, Direction)) {
    split = G4UniformRand();
    ++counter;
    if (counter > 100) {
      G4cout << " **** Was not able to split rectangle " << G4endl;
      return;
    }
  }
}

bool Surface::RectangleDivider::SplitRectangleAt(const G4int argRectangle,
                                                 const G4double argSplit,
                                                 const G4bool argDirection) {
  // Split one of the stored rectangles in two
  // Split Dimension, false splits in X direction, true in Y direction
  const Rectangle SelectedRec = fRectangle[argRectangle];
  const G4double SelectedFrac = fFraction[argRectangle];
  if (argDirection) {
    // split Y direction
    G4double diffY = (SelectedRec.maxY - SelectedRec.minY);
    if (diffY * argSplit < fminSize || diffY * (1. - argSplit) < fminSize) {
      return false;
    }
    diffY *= argSplit;
    fRectangle.emplace_back(Rectangle{SelectedRec.minX, SelectedRec.maxX,
                                      SelectedRec.minY + diffY,
                                      SelectedRec.maxY});
    fRectangle[argRectangle].maxY = SelectedRec.minY + diffY;
  } else {
    // split X direction
    G4double diffX = (SelectedRec.maxX - SelectedRec.minX);
    if (diffX * argSplit < fminSize || diffX * (1. - argSplit) < fminSize) {
      return false;
    }
    diffX *= argSplit;
    fRectangle.emplace_back(Rectangle{SelectedRec.minX + diffX,
                                      SelectedRec.maxX, SelectedRec.minY,
                                      SelectedRec.maxY});
    fRectangle[argRectangle].maxX = SelectedRec.minX + diffX;
  }
  fFraction.emplace_back(SelectedFrac * (1. - argSplit));
  fFraction[argRectangle] *= argSplit;
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
  return;
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
  return fRectangle.size();
}
