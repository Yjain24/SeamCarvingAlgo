#include "seam_carver.hpp"
#include "image_ppm.hpp"
// given functions below, DO NOT MODIFY




SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
// implement the rest of SeamCarver's functions here

const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }


Pixel SeamCarver::GetLeftPixel(int row, int col) const {
  return (col == 0) ? image_.GetPixel(row, width_ - 1) : image_.GetPixel(row, col - 1);
}

Pixel SeamCarver::GetRightPixel(int row, int col) const {
  return (col == width_ - 1) ? image_.GetPixel(row, 0) : image_.GetPixel(row, col + 1);
}

Pixel SeamCarver::GetUpPixel(int row, int col) const {
  return (row == 0) ? image_.GetPixel(height_ - 1, col) : image_.GetPixel(row - 1, col);
}

Pixel SeamCarver::GetDownPixel(int row, int col) const {
  return (row == height_ - 1) ? image_.GetPixel(0, col) : image_.GetPixel(row + 1, col);
}

int SeamCarver::CalculateEnergy(const Pixel& left, const Pixel& right, const Pixel& up, const Pixel& down) const {
  int energyred = CalculateColorDifference(left.GetRed(), right.GetRed()) + CalculateColorDifference(up.GetRed(), down.GetRed());
  int energygreen = CalculateColorDifference(left.GetGreen(), right.GetGreen()) + CalculateColorDifference(up.GetGreen(), down.GetGreen());
  int energyblue = CalculateColorDifference(left.GetBlue(), right.GetBlue()) + CalculateColorDifference(up.GetBlue(), down.GetBlue());

  return energyred + energygreen + energyblue;
}

int SeamCarver::CalculateColorDifference(int a, int b) const {
  return (a - b) * (a - b);
}
int SeamCarver::GetEnergy(int row, int col) const {
  Pixel left = GetLeftPixel(row, col);
  Pixel right = GetRightPixel(row, col);
  Pixel up = GetUpPixel(row, col);
  Pixel down = GetDownPixel(row, col);

  int energy = CalculateEnergy(left, right, up, down);

  return energy;
}




int* SeamCarver::GetHorizontalSeam() const {
  //int** horizontal_seams = (width_ == 0 || height_ == 0) ? nullptr : new int*[height_];
  int** horizontal_seams = new int*[height_];
  for (int row = 0; row < height_; row++) {
  //while (horizontal_seams!= nullptr && row < height_) {
    horizontal_seams[row] = new int[width_];
    //row++;
  }
  int row =0;
  while (horizontal_seams != nullptr && row < height_) {
    horizontal_seams[row][width_ - 1] = GetEnergy(row, width_ - 1);
    row++;
  }
  if (horizontal_seams != nullptr) {
    CalculateHorizontalSeamDP(horizontal_seams);
  }
  int* p = Helper(horizontal_seams);
  FreeMemory(horizontal_seams);
  return p;
}



void SeamCarver::CalculateHorizontalSeamDP(int** horizontal_seams) const {
  int col = width_ - 2;
  while (col >= 0) {
    int row = 0;
    while (row < height_) {
      int min = horizontal_seams[row][col + 1];
      UpdateMinEnergy(row - 1, col + 1, min, horizontal_seams);
      UpdateMinEnergy(row, col + 1, min, horizontal_seams);
      UpdateMinEnergy(row + 1, col + 1, min, horizontal_seams);
      horizontal_seams[row][col] = min + GetEnergy(row, col);
      row++;
    }
    col--;
  }
}


void SeamCarver::UpdateMinEnergy(int row, int col, int& min_energy, int** horizontal_seams) const {
  auto update_min = [&](int r) {
    if (r < 0 || r >= height_) {
      return;
    }
    int energy = horizontal_seams[r][col];
    if (energy < min_energy) {
      min_energy = energy;
    }
  };
  update_min(row - 1);
  update_min(row);
  update_min(row + 1);
}


void SeamCarver::FreeMemory(int** horizontal_seams) const {
  for (int row = 0; row < height_; row++) {
    delete[] horizontal_seams[row];
  }
  delete[] horizontal_seams;
  horizontal_seams = nullptr;
  // while (row < height_) {
  //   delete[] horizontal_seams[row];
  //   row++;
  // }
}

// void SeamCarver::CleanupVerticalSeams(int** vertical_seams) const {
//   for (int row = 0; row < height_; row++) {
//     delete[] vertical_seams[row];
//   }
//   delete[] vertical_seams;
// }



int SeamCarver::FindMinRowIndex(int** horizontal_seams, int col, int start_row_index, int height) const {
  int min_value = horizontal_seams[start_row_index][col];
  int min_row_index = start_row_index;

  auto update_min = [&](int value, int row_index) {
    if (value < min_value) {
      min_value = value;
      min_row_index = row_index;
    }
  };

  if (start_row_index > 0) {
    int value_above = horizontal_seams[start_row_index - 1][col];
    update_min(value_above, start_row_index - 1);
  }

  if (start_row_index < height - 1) {
    int value_below = horizontal_seams[start_row_index + 1][col];
    update_min(value_below, start_row_index + 1);
  }

  return min_row_index;
}



int SeamCarver::FindMinRowIndexFirstColumn(int** horizontal_seams, int height) const {
  int min_value = horizontal_seams[0][0];
  int min_row_index = 0;

  for (int row = 1; row < height; row++) {
    min_row_index = horizontal_seams[row][0] < min_value ? row : min_row_index;
    min_value = horizontal_seams[row][0] < min_value ? horizontal_seams[row][0] : min_value;
  }

  return min_row_index;
}

int* SeamCarver::GetMinSeamIndices(int** horizontal_seams, int height, int width) const {
  int* min_seam_indices = new int[width];
  int min_row_index = FindMinRowIndexFirstColumn(horizontal_seams, height);
  min_seam_indices[0] = min_row_index;

  for (int col = 1; col < width; col++) {
    min_row_index = FindMinRowIndex(horizontal_seams, col, min_row_index, height);
    min_seam_indices[col] = min_row_index;
  }

  return min_seam_indices;
}

int* SeamCarver::Helper(int** horizontal_seams) const {
  int* min_seam_indices = GetMinSeamIndices(horizontal_seams, height_, width_);
  return min_seam_indices;
}

int* SeamCarver::GetVerticalSeam() const {
  if (width_ == 0 || height_ == 0) {
    int* empty = new int[1]{0};
    return empty;
  }

  int** vertical_seams = new int*[height_];
  // int* verticel;
  for (int row = 0; row < height_; row++) {
    vertical_seams[row] = new int[width_];
  }

  for (int col = 0; col < width_; col++) {
    vertical_seams[height_ - 1][col] = GetEnergy(height_ - 1, col);
  }

  CalculateVerticalSeams(vertical_seams);

  int index = FindVerticalSeamIndex(vertical_seams);
  int* verticel = VerticalHelper(vertical_seams, index);

  CleanupVerticalSeams(vertical_seams);
  return verticel;
}

void SeamCarver::CalculateVerticalSeams(int** vertical_seams) const {
  auto get_min = [&](int row, int col) {
    int min = vertical_seams[row + 1][col];
    if (col > 0) {
      min = std::min(min, vertical_seams[row + 1][col - 1]);
    }
    if (col < width_ - 1) {
      min = std::min(min, vertical_seams[row + 1][col + 1]);
    }
    return min;
  };

  for (int row = height_ - 2; row >= 0; row--) {
    for (int col = 0; col < width_; col++) {
      int min = get_min(row, col);
      vertical_seams[row][col] = min + GetEnergy(row, col);
    }
  }
}

int SeamCarver::FindVerticalSeamIndex(int** vertical_seams) const {
  int value = vertical_seams[0][0];
  int index = 0;
  for (int col = 0; col < width_; col++) {
    if (value > vertical_seams[0][col]) {
      value = vertical_seams[0][col];
      index = col;
    }
  }
  return index;
}

void SeamCarver::CleanupVerticalSeams(int** vertical_seams) const {
  for (int row = 0; row < height_; row++) {
    delete[] vertical_seams[row];
  }
  delete[] vertical_seams;
  vertical_seams = nullptr;
}

int* SeamCarver::VerticalHelper(int** vertical_seams, int index) const {
  int* verticel = new int[height_];
  verticel[0] = index;
  int row = 1;
  while (row < height_) {
    index = FindMinAdjacent(vertical_seams, row, index);
    verticel[row] = index;
    row++;
  }
  return verticel;
}

int SeamCarver::FindMinAdjacent(int** vertical_seams, int row, int col) const {
  int min_value = vertical_seams[row][col];
  int min_index = col;
  if (col < width_ - 1 && vertical_seams[row][col + 1] < min_value) {
    min_value = vertical_seams[row][col + 1];
    min_index = col + 1;
  }
  if (col > 0 && vertical_seams[row][col - 1] < min_value) {
    min_value = vertical_seams[row][col - 1];
    min_index = col - 1;
  }
  return min_index;
}

















void SeamCarver::RemoveVerticalSeam() {
  int* temp = GetVerticalSeam();
  image_.RemoveVerticalCarve(temp);
  delete[] temp;
  temp = nullptr;
  width_--;
}


void SeamCarver::RemoveHorizontalSeam() {
  if (height_ == 1) return;
  int* seam = GetHorizontalSeam();
  image_.RemoveHorizontal(seam);
  SetImage(this->GetImage());
  delete[] seam;
  seam = nullptr;
}