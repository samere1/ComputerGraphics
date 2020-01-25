/** @file PPM.h
 *  @brief Class for working with PPM images
 *
 *  Class for working with P3 PPM images specifically.
 *
 *  @author your_name_here
 *  @bug No known bugs.
 */
#ifndef PPM_H
#define PPM_H

#include <string>
#include <vector>

class PPM {
public:
  // Constructor loads a filename with the .ppm extension
  PPM(std::string fileName);
  // Destructor clears any memory that has been allocated
  ~PPM();
  // Saves a PPM Image to a new file.
  void savePPM(std::string outputFileName);
  // Darken subtracts 50 from each of the red, green
  // and blue color components of all of the pixels
  // in the PPM. Note that no values may be less than
  // 0 in a ppm.
  void darken();
  // Sets a pixel to a specific R,G,B value 
  void setPixel(int x, int y, int r, int g, int b);

  // Returns the raw pixel data in an array.
  inline unsigned char *pixelData() { return m_PixelData; }
  // Returns image width
  inline int getWidth() { return m_width; }
  // Returns image height
  inline int getHeight() { return m_height; }
private:
  // Use a token from the first part of the PPM file to set the width,
  // height (and initial pixel data), or max color value.
  void parseMetadata(std::string token, int i, int &colorScale);
  // Use a token from the body of the PPM file to store an individual
  // color value, or save a pixel if we are at the end of a triple.
  void parseColorValues(std::string token, int i, int colorScale,
    int &r, int &g, unsigned int &x, unsigned int &y);
  // Subtracts 50 from an individual color component,
  // without making it go below 0.
  int darkenColor(int colorValue);
  // Calculates the index of the R color component in the
  // pixel data based on pixel's x and y coordinates.
  int getRIndex(int x, int y);

  // Store the raw pixel data here
  // Data is R,G,B format
  unsigned char *m_PixelData;
  // Store width and height of image
  int m_width{ 0 };
  int m_height{ 0 };
};


#endif
