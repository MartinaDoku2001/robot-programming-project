
#include "grid_map.h"
#include "grid.h"
#include <map_utils.h>
#include <cstring>
using namespace std;



GridMap::GridMap(int rows_, int cols_, float resolution_):
  Grid_<uint8_t>(rows_, cols_),
  resolution(resolution_)
{
  gm.resize(rows_, cols_, resolution);
}

bool GridMap::scanRay(Vector2f& hit,
                      const Vector2f& origin,
                      const Vector2f& direction,
                      const float max_range) const{
  float range = 0;
  while (range < max_range) {
    hit = origin + direction * range;
    Vector2f grid_endpoint = gm.world2grid(hit);
    int r=grid_endpoint.y();
    int c=grid_endpoint.x();
    range += resolution;
    if (!inside(r,c)) return false;
    uint8_t val = at(r,c);
    if (val < 127) {
      return true;
    }
  }
  return false;
}

float GridMap::scanRayDistance(const Vector2f& origin,
                        const Vector2f& direction,
                        const float max_range) const {
  float range = 0;

  while (range < max_range) {
    Vector2f grid_endpoint = gm.world2grid(origin + direction * range);
    int r=grid_endpoint.y();
    int c=grid_endpoint.x();
    if (!inside(r,c))
      return max_range;
    if (at(r,c) < 127)
      return range;
    range +=resolution;
  }
  
  return max_range;
}

void GridMap::loadFromImage(std::string filename, float res) {
  resolution = res;
  cerr << "loading [" << filename << "]" << endl;
  cv::Mat m = cv::imread(filename);
  if (m.rows == 0) {
    throw std::runtime_error("unable to load image");
  }
  cv::Mat loaded_image;
  cv::cvtColor(m, loaded_image, cv::COLOR_BGR2GRAY);
  int size = loaded_image.rows * loaded_image.cols;
  resize(loaded_image.rows, loaded_image.cols);
  gm.resize(rows, cols, res);
  cerr << "transform: " << endl;
  cerr << gm.world_to_grid.matrix() << endl;
  
  memcpy(&cells[0], loaded_image.data, size);
}

void GridMap::draw(Canvas& dest) const {
  drawGrid(dest, *this);
}

bool GridMap::is_colliding(float x, float y) const {
  //x and y are the robot position in world coordinates

  // Convert the (x, y) world coordinates to grid coordinates
  Vector2f point(x, y);
  Vector2f grid_point = gm.world2grid(point);

  // Extract row and column from the grid point
  int r = grid_point.y();
  int c = grid_point.x();

  // check if the cell value (or any cell value in the vicinity) is below the obstacle threshold (127)
  for (int rr = r - 3; rr <= r + 3; ++rr) {
    for (int cc = c - 3; cc <= c + 3; ++cc) {
      if (at(rr, cc) < 127)
        return true;
    }
  }

  return false;
}


