#pragma once
#include <vector>
#include <Eigen/Core>

using Eigen::Vector2f;
using Eigen::Vector2i;

/* create the GRID structure */
template <typename CellType_>
struct Grid_ {
    // define names for the structure elements
    using CellType=CellType_; 
    using ContainerType=std::vector<CellType>;

    // define the structure elements
    int rows;
    int cols;
    ContainerType cells;

    // constructor
    Grid_(int rows_, int cols_) :
     rows(rows_), 
     cols(cols_), 
     cells(rows*cols) {};
};