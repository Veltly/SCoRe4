
#include "SurfaceGenerator/include/Generator.hh"
#include <iostream>
int main() {
  Surface::SurfaceGenerator *generator = new Surface::SurfaceGenerator();
  std::cout << "Surface Generator created" << std::endl;
  return 0;
}
