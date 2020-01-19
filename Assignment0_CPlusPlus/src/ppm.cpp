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
    // Loop through tokens on each line
    std::stringstream strstream(line);
    std::string token;
    while (getline(strstream, token, ' ')) {
      // Ignore whitespace
      if (token == "") continue;

      // Ignore the rest of the line if there is a comment
      if (token.at(0) == '#') {
        strstream.str("");
        continue;
      }

      // Check that the PPM format is P3
      if (i == 0) {
        if (token != "P3") {
          std::cout << "File is not in PPM P3 format." << std::endl;
          return;
        }
      }
      else {
        // Set width, height, initial pixel data, and max color value
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
        // Find RGB values
        else {
          int colorIndex = (i - 4) % 3;
          if (colorIndex == 0) {
            r = tokenInt * colorScale;
          }
          else if (colorIndex == 1) {
            g = tokenInt * colorScale;
          }
          else if (colorIndex == 2) {
            setPixel(x, y, r, g, tokenInt * colorScale);
            if (++x == m_width) {
              x = 0;
              ++y;
            }
          }
        }
      }
      ++i;
    }
  }
  inFile.close();
}

// Destructor clears any memory that has been allocated
PPM::~PPM() {
  delete[] m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName) {
  std::ofstream outFile;
  outFile.open(outputFileName);
  outFile << "P3" << std::endl;
  outFile << m_width << " " << m_height << std::endl;
  outFile << MAX_COLOR_VALUE << std::endl;
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

// Subtracts 50 from an individual color component,
// without making it go below 0.
int PPM::darkenColor(int colorValue) {
  return (colorValue > 50) ? (colorValue - 50) : 0;
}