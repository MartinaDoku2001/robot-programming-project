#pragma once
#include <cstdint>

#include "grid.h"
#include "map_utils.h"

using namespace std;

/**
 * @brief Represents a grid map.
 * 
 * GridMap is a class that represents a grid map
 * with obstacle information and provides functionalities
 * to load a map from an image, scan rays, and draw the map.
 * 
 */
struct GridMap: public Grid_<uint8_t> {
  GridMapping gm;
  float resolution;      // meters*pixel
  GridMap(int rows, int cols, float resolution);

  /**
   * @brief Loads the map from an image file.
   * 
   * This function loads the map from an image file
   * and sets the resolution of the map.
   * 
   * @param filename The name of the image file.
   * @param resolution The resolution of the map in meters per pixel.
   * 
   * @return void
   */
  void loadFromImage(std::string filename, float resolution);

  /**
   * @brief Scans a ray in the map.
   * 
   * This function scans a ray in the map
   * 
   * @param hit The hit point of the ray.
   * @param origin The origin of the ray.
   * @param direction The direction of the ray.
   * @param max_range The maximum range of the ray.
   * 
   * @return True if the ray hits an obstacle, false otherwise.
   */
  bool scanRay(Vector2f& hit,
               const Vector2f& origin,
               const Vector2f& direction,
               const float max_range) const;

  /**
   * @brief Scans a ray in the map.
   * 
   * This function scans a ray in the map
   * 
   * @param origin The origin of the ray.
   * @param direction The direction of the ray.
   * @param max_range The maximum range of the ray.
   * 
   * @return The range of the ray.
   */
  float scanRay(const Vector2f& origin,
                 const Vector2f& direction,
                 const float max_range) const;
  
  /**
   * @brief Draws the map on a canvas.
   * 
   * This function draws the map on a canvas.
   * 
   * @param dest The canvas to draw the map on.
   * 
    * @return void
    */
  void draw(Canvas& dest) const;
};
