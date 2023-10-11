#include "image_ppm.hpp"

// implement the rest of ImagePPM's functions here

// given functions below, DO NOT MODIFY

ImagePPM::ImagePPM(const std::string& path) {
  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }

  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}

std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;
  // ignore magic number line
  getline(is, line);
  // check to see if there's a comment line
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  // parse width and height
  int space = line.find_first_of(' ');
  image.width_ = std::stoi(line.substr(0, space));
  image.height_ = std::stoi(line.substr(space + 1));
  // get max color value
  getline(is, line);
  image.max_color_value_ = std::stoi(line);
  // init and fill in Pixels array
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  return is;
}

Pixel ImagePPM::GetPixel(int row, int col) const { return pixels_[row][col]; }

int ImagePPM::GetMaxColorValue() const { return max_color_value_; }

void ImagePPM::SetImage(int height,
                        int width,
                        int max_color,
                        Pixel** pixels) {
  for (int row = 0; row < height_; ++row) delete[] pixels_[row];
  delete[] pixels_;
  pixels_ = nullptr;
  height_ = height, width_ = width;
  max_color_value_ = max_color;
  pixels_ = pixels;
}


void ImagePPM::RemoveVerticalCarve(const int* seam) {
  Pixel** temp = AllocatePixelArray(height_, width_ - 1);
  CopyPixelsUpToSeam(pixels_, temp, height_, seam);
  ShiftPixelsAfterSeam(pixels_, temp, height_, width_, seam);
  DeletePixelArray(pixels_, height_);
  pixels_ = temp;
  width_--;
}

Pixel** ImagePPM::AllocatePixelArray(int height, int width) {
  auto** pixels = new Pixel*[height];
  for (int row = 0; row < height; row++) {
    pixels[row] = new Pixel[width];
  }
  return pixels;
}

void ImagePPM::DeletePixelArray(Pixel** pixels, int height) {
  for (int row = 0; row < height; row++) {
    delete[] pixels[row];
  }
  delete[] pixels;
}

void ImagePPM::CopyPixelsUpToSeam( Pixel** source, Pixel** destination, int height, const int* seam) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < seam[row]; col++) {
      destination[row][col] = source[row][col];
    }
  }
}

void ImagePPM::ShiftPixelsAfterSeam( Pixel** source, Pixel** destination, int height, int width, const int* seam) {
  for (int row = 0; row < height; row++) {
    for (int col = seam[row] + 1; col < width; col++) {
      destination[row][col - 1] = source[row][col];
    }
  }
}





// void ImagePPM::RemoveVertical(int height, Pixel** image, int width) {
//   Clear();
//   pixels_ = image;
//   height_ = height;
//   width_ = width;
// }





Pixel** ImagePPM::CreateNewPixelArray(int height, int width) {
  auto** new_pix = new Pixel*[height];
  for (int row = 0; row < height; row++) {
    new_pix[row] = new Pixel[width];
  }
  return new_pix;
}


void ImagePPM::CopyPixelsWithRowRemoved(int*& to_remove, int new_height, int width, Pixel** old_pix, Pixel** new_pix) {
  for (int col = 0; col < width; col++) {
    int image_row = 0;
    for (int row = 0; row < new_height; row++) {
      if (row == to_remove[col]) {
        image_row++;
      }
      new_pix[row][col] = old_pix[image_row][col];
      image_row++;
    }
  }
}


void ImagePPM::RemoveHorizontal(int*& to_remove) {
  int new_height = height_ - 1;
  Pixel** new_pix = CreateNewPixelArray(new_height, width_);
  CopyPixelsWithRowRemoved(to_remove, new_height, width_, pixels_, new_pix);
  DeletePixels();
  height_--;
  pixels_ = new_pix;
}


void ImagePPM::DeletePixels() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }
  delete[] pixels_;
}










std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  os << "P3" << std::endl;
  os << image.width_ << " " << image.height_ << std::endl << image.max_color_value_ << std::endl;
  for (int row = 0; row < image.height_; ++row) {
    for (int column = 0; column < image.width_; ++column) {
      os << image.GetPixel(row, column).GetRed() << "\n";
      os << image.GetPixel(row, column).GetGreen() << "\n";
      os << image.GetPixel(row, column).GetBlue() << "\n";
    }
  }
  return os;
}