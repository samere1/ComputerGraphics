#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "PPM.h"

const int MAX_COLOR_VALUE = 255;

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName) {
  // Only open files with .ppm extension
  if (fileName.substr(fileName.rfind('.')) != ".ppm") {
    std::cout << "Please provide a file with the .ppm extension" << std::endl;
    return;
  }

  // Attempt to open the file
  std::ifstream inFile;
  inFile.open(fileName);
  if (!inFile.is_open()) {
    std::cout << "Unable to open file " << fileName << std::endl;
    return;
  }

  unsigned int i = 0, x = 0, y = 0;
  int r, g, colorScale = 1;
  std::string line;
  while (getline(inFile, line)) {
    // Check for comments
    int commentIndex = line.find("#");
    if (commentIndex != std::string::npos) {
      line = line.substr(0, commentIndex);
    }

    // Loop through values as tokens (handles multiple values on one line)
    std::stringstream strstream(line);
    std::string token;
    while (getline(strstream, token, ' ')) {
      if (token == "") continue; // Ignore blank tokens caused by consecutive spaces
      if (i == 0) {
        // Check that the PPM format is P3
        if (token != "P3") {
          std::cout << "File is not in PPM P3 format." << std::endl;
          return;
        }
      }
      else if (i <= 3) {
        parseMetadata(token, i, colorScale);
      }
      else {
        parseColorValues(token, i, colorScale, r, g, x, y);
      }
      ++i;
    }
  }
  inFile.close();
}

// Use a token from the first part of the PPM file to set the width,
// height (and initial pixel data), or max color value.
void PPM::parseMetadata(std::string token, int i, int &colorScale) {
  int tokenInt = std::stoi(token);
  if (i == 1) {
    m_width = tokenInt;
  }
  else if (i == 2) {
    m_height = tokenInt;
    m_PixelData = new unsigned char[m_width * m_height * 3];
  }
  else if (i == 3) {
    colorScale = MAX_COLOR_VALUE / tokenInt;
  }
}

// Use a token from the body of the PPM file to store an individual
// color value, or save a pixel if we are at the end of a triple.
void PPM::parseColorValues(std::string token, int i, int colorScale,
  int &r, int &g, unsigned int &x, unsigned int &y) {
  int colorIndex = (i - 4) % 3; // minus four to compensate for metadata
  int colorValue = std::stoi(token) * colorScale;
  if (colorIndex == 0) {
    r = colorValue;
  }
  else if (colorIndex == 1) {
    g = colorValue;
  }
  else if (colorIndex == 2) {
    setPixel(x, y, r, g, colorValue);
    if (++x == m_width) {
      x = 0;
      ++y;
    }
  }
}

// Destructor clears any memory that has been allocated
PPM::~PPM() {
  delete[] m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName) {
  std::ofstream outFile;
  outFile.open(outputFileName);

  // Print header, width, height, max color value
  outFile << "P3" << std::endl;
  outFile << m_width << " " << m_height << std::endl;
  outFile << MAX_COLOR_VALUE << std::endl;

  // Print color values with each row on its own line
  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      int rIndex = getRIndex(x, y);
      int r = m_PixelData[rIndex];
      int g = m_PixelData[rIndex + 1];
      int b = m_PixelData[rIndex + 2];
      outFile << r << " " << g << " " << b << " ";
    }
    outFile << std::endl;
  }
  outFile.close();
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken() {
  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      int rIndex = getRIndex(x, y);
      setPixel(x, y,
        darkenColor(m_PixelData[rIndex]),
        darkenColor(m_PixelData[rIndex + 1]),
        darkenColor(m_PixelData[rIndex + 2]));
    }
  }
}

// Subtracts 50 from an individual color component,
// without making it go below 0.
int PPM::darkenColor(int colorValue) {
  return (colorValue > 50) ? (colorValue - 50) : 0;
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int r, int g, int b) {
  int rIndex = getRIndex(x, y);
  m_PixelData[rIndex] = r;
  m_PixelData[rIndex + 1] = g;
  m_PixelData[rIndex + 2] = b;
}

// Calculates the index of the R color component in the
// pixel data based on pixel's x and y coordinates.
int PPM::getRIndex(int x, int y) {
  return ((y * m_width) + x) * 3;
}
