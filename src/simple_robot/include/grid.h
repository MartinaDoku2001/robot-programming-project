#pragma once
#include <vector>
#include <cmath>
#include <eigen3/Eigen/Core>

using Eigen::Vector2f;
using Eigen::Vector2i;

template <typename CellType_>
/**
 * @brief Represents a grid of cells.
 * 
 * Grid_ is a templated class that represents a grid 
 * of cells of type CellType_.
 * 
 * @tparam CellType_ The type of the cells in
 *  the grid.
 */
struct Grid_ {
  using CellType=CellType_;
  using ContainerType=std::vector<CellType>;
  int rows;
  int cols;
  ContainerType cells;
  Grid_(int r=0, int c=0):
    rows(r),
    cols(c),
    cells(r*c){}

  /**
   * @brief Resizes the grid to the given dimensions.
   * 
   * This function resizes the grid to the 
   * given dimensions.
   * 
   * @param new_r The new number of rows.
   * @param new_c The new number of columns.
   * 
   * @return void
   */
  void resize(int new_r, int new_c) {
    if (new_r==rows && new_c==cols)
      return;
    rows=new_r;
    cols=new_c;
    cells.resize(rows*cols);
  }
  
  /**
   * @brief Accesses the cell at the given row and column.
   * 
   * This function accesses the cell at the given row
   * and column.
   * 
   * @param r The row of the cell.
   * @param c The column of the cell.
   * 
   * @return A reference to the cell at the given row and column.
   */
  inline CellType& at(int r, int c){
    return cells[r*cols+c];
  };

  /**
   * @brief Accesses the cell at the given row and column.
   * 
   * This function accesses the cell at the given row
   * and column.
   * 
   * @param r The row of the cell.
   * @param c The column of the cell.
   * 
   * @return A reference to the cell at the given row and column.
   */
  inline const CellType& at(int r, int c) const {
    return cells[r*cols+c];
  };

  /**
   * @brief Accesses the cell at the given row and column.
   * 
   * This function accesses the cell at the given row
   * and column.
   * 
   * @param r The row of the cell.
   * @param c The column of the cell.
   * 
   * @return A reference to the cell at the given row and column.
   */
  inline bool inside(int r, int c) const {
    return r>=0 && r<rows && c>=0 && c<cols;
  }

  // float accessors with interpolation, it does not return a writable cell
  inline bool inside(const Eigen::Vector2f& px) const {
    return px.x()>=1 && px.x()<cols-1 && px.y()>=1 && px.y()<rows-1;
  }

  /*https://en.wikipedia.org/wiki/Bilinear_interpolation*/
  /**
   * @brief Accesses the grid with bilinear interpolation.
   * 
   * This function accesses the grid with bilinear 
   * interpolation that means it computes the value
   * of the cell at the given position by interpolating
   * the values of the four nearest cells.
   * 
   * @param px The position to access.
   */
  inline const CellType at(const Eigen::Vector2f& px) const {
    const float r=px.y();
    const float c=px.x();
    float dr=r-floor(r);
    float dc=c-floor(c);
    int r0=r-dr;
    int c0=c-dc;
    const CellType& f00=at(r0,c0);
    const CellType& f10=at(r0+1,c0);
    const CellType& f01=at(r0,c0+1);
    const CellType& f11=at(r0+1,c0+1);
    const CellType a00=f00;
    const CellType a10=f10-f00;
    const CellType a01=f01-f00;
    const CellType a11=f11-f10-f01+f00;
    return a00+a10*dr+a01*dc+a11*dr*dc;
  }
  
  inline std::pair<int, int> ptr2idx(const CellType* c) const {
      const int offset=c-&cells[0];
      return std::make_pair(offset/cols, offset%cols);
  }

 
  /**
   * @brief Computes the squared distance between two
   *  cells.
   * 
   * @param c1 The first cell.
   * @param c2 The second cell.
   * 
   * @return The squared distance between the two cells.
   */
  inline int distance2(const CellType&c1,
                       const CellType&c2) const {
        auto c1_pos=ptr2idx(&c1);
        auto c2_pos=ptr2idx(&c2);
        int dr=c1_pos.first-c2_pos.first;
        int dc=c1_pos.second-c2_pos.second;
        return dr*dr+dc*dc;
  }

  /**
   * @brief Fills the grid with the given value.
   * 
   * @param c The value to fill the grid with.
   */
  void fill (const CellType& c) {
    std::fill(cells.begin(), cells.end(), c);
  }

  /**
   * @brief Computes the derivative of the grid along the rows.
   * 
   * @param dest The grid to store the derivative in.
   */
  void rowDerivative(Grid_<CellType>& dest) const {
    dest.resize(rows, cols);
    for (int r=1; r<rows-1; ++r)
      for (int c=1; c<cols-1; ++c)
        dest.at(r,c)=at(r+1,c)-at(r-1,c);
  }

  /**
   * @brief Computes the derivative of the grid along the columns.
   * 
   * @param dest The grid to store the derivative in.
   */
  void colDerivative(Grid_<CellType>& dest) const {
    dest.resize(rows, cols);
    for (int r=1; r<rows-1; ++r)
      for (int c=1; c<cols-1; ++c)
        dest.at(r,c)=at(r,c+1)-at(r,c-1);
  }

  
  template <typename OtherCellType_>

  /**
   * @brief Copies the grid to another grid.
   * 
   * @param dest The grid to copy to.
   */
  void copyTo(Grid_<OtherCellType_>& dest) const {
    dest.resize(rows, cols);
    for (size_t i=0; i<cells.size(); ++i) {
      dest.cells[i]=cells[i];
    }
  }
};

