#pragma once
#include <cstdint>

#include "grid.h"
#include "map_utils.h"

using namespace std;

// grid mapping class
struct GridMap: public Grid_<uint8_t> {
  GridMapping gm;
  float resolution;      // meters*pixel
  GridMap(int rows, int cols, float resolution);

  // loads a map from an image
  void loadFromImage(std::string filename, float resolution);

  bool scanRay(Vector2f& hit,
               const Vector2f& origin,
               const Vector2f& direction,
               const float max_range) const;

  float scanRay(const Vector2f& origin,
                 const Vector2f& direction,
                 const float max_range) const;
    
  void draw(Canvas& dest) const;
};
