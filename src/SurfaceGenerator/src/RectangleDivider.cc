/*
 * RectangleDivider.cc
 *
 *  Created on: May 14, 2023
 *      Author: cgruner
 */
#include "../include/RectangleDivider.hh"
#include "G4ios.hh"
#include "Randomize.hh"
#include <G4Types.hh>
#include <fstream>

Surface::RectangleDivider::RectangleDivider(G4double minSize,
                                                        G4double minX,
                                                        G4double maxX,
                                                        G4double minY,
                                                        G4double maxY)
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

bool Surface::RectangleDivider::SplitRectangleAt(
    G4int argRectangle, G4double argSplit, G4bool argDirection) {
  // Split one of the stored rectangles in two
  // Split Dimension, false splits in X direction, true in Y direction
  Rectangle SelectedRec = fRectangle[argRectangle];
  G4double SelectedFrac = fFraction[argRectangle];
  if (argDirection) {
    // split Y direction
    G4double diffY = (SelectedRec.maxY - SelectedRec.minY);
    if (diffY * argSplit < fminSize or diffY * (1. - argSplit) < fminSize) {
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
    if (diffX * argSplit < fminSize or diffX * (1. - argSplit) < fminSize) {
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

void Surface::RectangleDivider::SplitRectangleEven(G4int argX,
                                                               G4int argY) {
  Rectangle bigRectangle = fRectangle[0];
  fRectangle.pop_back();
  fFraction.pop_back();
  G4double lenX = (bigRectangle.maxX - bigRectangle.minX) / argX;
  G4double lenY = (bigRectangle.maxY - bigRectangle.minY) / argY;
  for (G4int x = 0; x < argX; ++x) {
    for (G4int y = 0; y < argY; ++y) {
      G4double minX = bigRectangle.minX + x * lenX;
      G4double maxX = bigRectangle.minX + (x + 1) * lenX;
      G4double minY = bigRectangle.minY + y * lenX;
      G4double maxY = bigRectangle.minY + (y + 1) * lenY;
      fRectangle.emplace_back(Rectangle{minX, maxX, minY, maxY});
      fFraction.push_back(1. / (argX * argY));
    }
  }
}

void Surface::RectangleDivider::SplitRectangle(G4int SplitNRounds) {
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

G4int Surface::RectangleDivider::GetNumberOfRectangles() {
  return fRectangle.size();
}
