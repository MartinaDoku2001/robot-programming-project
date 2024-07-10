#pragma once
#include "grid.h"
#include <limits>

using namespace std;

/**
 * @brief Represents a cell in the distance map.
 * 
 * DMapCell holds information about each cell in the distance map,
 * including its parent pointer for pathfinding and any other relevant data.
 */
struct DMapCell {
    DMapCell* parent = nullptr;
};



/**
 * @brief Distance map class for obstacle propagation and pathfinding.
 * 
 * DMap provides functionalities to compute the distance map based on
 * obstacles and clear the map. It uses a breadth-first search (BFS)
 * algorithm for propagation.
 */
struct DMap: public Grid_<DMapCell> {

  DMap(int rows, int cols):
    Grid_<DMapCell>(rows,cols){}

  /**
     * @brief Computes the distance map based on obstacles.
     * 
     * This function initializes the frontier with obstacles and performs
     * a BFS to propagate values, updating cells within the distance
     * squared threshold.
     * 
     * @param obstacles A vector of (row, column) pairs representing obstacle positions.
     * @param d2_max The maximum allowed distance squared for propagation.
     * @return The number of steps taken in the BFS.
     */
  int compute(const std::vector<std::pair<int,int>>& obstacles,
              int d2_max=std::numeric_limits<int>::max()); 
  
  /**
     * @brief Clears the distance map.
     * 
     * Resets the distance map by filling it with default DMapCell values.
     */
  void clear();

  template <typename OutputCellType>

  /**
   * @brief extracts the distance values (up to d2_max) and copies it to another grid.
   * 
   */
  void copyTo(Grid_<OutputCellType>& dest, int d2_max=std::numeric_limits<int>::max()) {
    dest.resize(rows, cols);
    for (size_t i=0; i<cells.size(); ++i) { // iterate over all cells
      const auto& cell=cells[i];
      int d2=d2_max;
      if(cell.parent){ // if the cell has a parent
        d2=distance2(cell, *cell.parent); // calculate the distance squared
      }
      dest.cells[i]=d2; // copy the distance squared to the target
    }
  }
  
};

