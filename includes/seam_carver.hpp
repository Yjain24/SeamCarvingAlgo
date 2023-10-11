#ifndef SEAM_CARVER_HPP
#define SEAM_CARVER_HPP

#include "seam_carver.hpp"
#include "image_ppm.hpp"
#include "pixel.hpp" 
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <sstream>

class SeamCarver {
public:
  // default constructor
  //
  // Not tested, but you are welcome to intialize (or not) member variables
  // however you like in this function
  SeamCarver() = default;

  // overloaded constructor
  //
  // sets the instances' image_ as a DEEP COPY of the image parameter. Note
  // that if ImagePPM's copy constructor is implemented correctly, you
  // shouldn't have to worry about the details of a deep copy here
  SeamCarver(const ImagePPM& image);

  // sets the instances' image_ as a DEEP COPY of the image parameter. Same
  // note as above regarding ImagePPM
  void SetImage(const ImagePPM& image);

  // returns the instance's image_
  const ImagePPM& GetImage() const;

  // returns the image's height
  int GetHeight() const;

  // returns the image's width
  int GetWidth() const;

  // returns the energy of the pixel at row col in image_
  int GetEnergy(int row, int col) const;
Pixel GetLeftPixel(int row, int col) const;

Pixel GetRightPixel(int row, int col) const;

Pixel GetUpPixel(int row, int col) const;

Pixel GetDownPixel(int row, int col) const;
int CalculateEnergy(const Pixel& left, const Pixel& right, const Pixel& up, const Pixel& down) const;
int CalculateColorDifference(int a, int b) const;

  // returns the horizontal seam of image_ with the least amount of
  // energy
  //
  // the ith int in the returned array corresponds to which row at
  // col i is in the seam. example:
  //
  //    | x |   |
  // ---+---+---+---
  //  x |   | x |
  // ---+---+---+---
  //    |   |   | x
  // returns {1, 0, 1, 2}
  int* GetHorizontalSeam() const;
 
void CalculateHorizontalSeamDP(int** horizontal_seams) const;
void UpdateMinEnergy(int row, int col, int& min_energy, int** horizontal_seams) const ;
void FreeMemory(int** horizontal_seams) const ;
int* Helper(int** horizontal_seams) const;




// Finds the index of the minimum value in the first column of the horizontal seam matrix
int FindMinRowIndexFirstColumn(int** horizontal_seams, int height) const;

// Returns an array of indices of the row with the minimum value in each column of the horizontal seam matrix
int* GetMinSeamIndices(int** horizontal_seams, int height, int width) const;
int FindMinRowIndex(int** horizontal_seams, int col, int start_row_index, int height) const;
  // returns the vertical seam of image_ with the least amount of
  // energy
  //
  // the ith int in the returned array corresponds to which col at
  // row i is in the seam. example:
  //
  //    | x |   |
  // ---+---+---+---
  //    |   | x |
  // ---+---+---+---
  //    |   | x |
  // returns {1, 2, 2}
  int* GetVerticalSeam() const;

  int* VerticalHelper(int** vertical_seams, int index) const ;
  void CleanupVerticalSeams(int** vertical_seams) const;
  int FindVerticalSeamIndex(int** vertical_seams) const;
  void CalculateVerticalSeams(int** vertical_seams) const;
  int FindMinAdjacent(int** vertical_seams, int row, int col) const;
  // removes one horizontal seam in image_. example:
  //
  // image_ before:
  //  0 | 1 | 2 | 3
  // ---+---+---+---
  //  4 | 5 | 6 | 7
  // ---+---+---+---
  //  8 | 9 | 10| 11
  //
  // seam to remove:
  //    | x |   |
  // ---+---+---+---
  //  x |   | x |
  // ---+---+---+---
  //    |   |   | x
  //
  // image_ after:
  //  0 | 5 | 2 | 3
  // ---+---+---+---
  //  8 | 9 | 10| 7
  void RemoveHorizontalSeam() ;

  

  // removes one vertical seam in image_. example:
  //
  // image_ before:
  //  0 | 1 | 2 | 3
  // ---+---+---+---
  //  4 | 5 | 6 | 7
  // ---+---+---+---
  //  8 | 9 | 10| 11
  //
  // seam to remove:
  //    | x |   |
  // ---+---+---+---
  //    |   | x |
  // ---+---+---+---
  //    |   | x |
  //
  // image_ after:
  //  0 | 2 | 3
  // ---+---+---
  //  4 | 5 | 7
  // ---+---+---
  //  8 | 9 | 11
  void RemoveVerticalSeam();

  /**
   * Add any helper methods you may need
   */

private:
  ImagePPM image_;
  int height_ = 0;
  int width_ = 0;

  /**
   * Add any helper methods you may need
   */
 
};

#endif