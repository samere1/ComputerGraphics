/** @file main.cpp
 *  @brief Entry point into our program.
 *
 *  Welcome to the Great Looking Software Render
 *  code base (Yes, I needed something with a gl prefix).
 *
 *  This is where you will implement your graphics API.
 *
 *  Compile on the terminal with:
 *
 *  clang++ -std=c++11 main.cpp -o main
 *
 *  @author Mike Shah
 *  @bug No known bugs.
 */

 // Some define values
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 320

// C++ Standard Libraries
#include <iostream>

// User libraries
#include "GL.h"
#include "Color.h"
#include "TGA.h"
#include "Maths.h"

// Create a canvas to draw on.
TGA canvas(WINDOW_WIDTH, WINDOW_HEIGHT);


// Implementation of Bresenham's Line Algorithm
// The input to this algorithm is two points and a color
// This algorithm will then modify a canvas (i.e. image)
// filling in the appropriate colors.
void drawLine(Vec2 v0, Vec2 v1, TGA& image, ColorRGB c) {
  bool steep = false;
  if (std::abs(v0.x - v1.x) < std::abs(v0.y - v1.y)) {
    // If the line is steep we want to transpose the image.
    std::swap(v0.x, v0.y);
    std::swap(v1.x, v1.y);
    steep = true;
  }
  if (v0.x > v1.x) {  // make it left-to-right
    std::swap(v0.x, v1.x);
    std::swap(v0.y, v1.y);
  }
  for (int x = v0.x; x <= v1.x; ++x) {
    float t = (x - v0.x) / (float)(v1.x - v0.x);
    int y = v0.y * (1.0f - t) + v1.y * t;
    if (steep) {
      canvas.setPixelColor(y, x, c);
    }
    else {
      canvas.setPixelColor(x, y, c);
    }
  }
}

float determinant(Vec2 v0, Vec2 v1) {
  return (v0.x * v1.y) - (v0.y * v1.x);
}

// Draw a triangle
void triangle(Vec2 v0, Vec2 v1, Vec2 v2, TGA& image, ColorRGB c) {
  if (glFillMode == LINE) {
    drawLine(v0, v1, image, c);
    drawLine(v1, v2, image, c);
    drawLine(v2, v0, image, c);
  }
  else if (glFillMode == FILL) { // Barycentric Algorithm
    // Bounding box of triangle
    int maxX = std::max(v0.x, std::max(v1.x, v2.x));
    int minX = std::min(v0.x, std::min(v1.x, v2.x));
    int maxY = std::max(v0.y, std::max(v1.y, v2.y));
    int minY = std::min(v0.y, std::min(v1.y, v2.y));

    // Spanning vectors of edges (v0, v1) and (v1, v2)
    Vec2 vs1 = Vec2(v1.x - v0.x, v1.y - v0.y);
    Vec2 vs2 = Vec2(v2.x - v0.x, v2.y - v0.y);

    // Loop through points in bounding box and draw the ones inside the triangle
    for (int x = minX; x <= maxX; x++) {
      for (int y = minY; y <= maxY; y++) {
        Vec2 q = Vec2(x - v0.x, y - v0.y);
        float s = determinant(q, vs2) / determinant(vs1, vs2);
        float t = determinant(vs1, q) / determinant(vs1, vs2);

        if ((s >= 0) && (t >= 0) && (s + t <= 1)) {
          canvas.setPixelColor(x, y, c);
        }
      }
    }
  }
}



// Main
int main() {
  // Set fill mode
  glPolygonMode(FILL);

  // Colors
  ColorRGB red; red.r = 255; red.g = 0; red.b = 0;
  ColorRGB lightBlue; lightBlue.r = 60; lightBlue.g = 170; lightBlue.b = 255;
  ColorRGB darkGreen; darkGreen.r = 0; darkGreen.g = 110; darkGreen.b = 40;

  // Triangle points
  Vec2 triangle1[3] = { Vec2(160, 60), Vec2(150, 10), Vec2(75, 190) };
  Vec2 triangle2[3] = { Vec2(200, 30), Vec2(250, 50), Vec2(220, 100) };
  Vec2 triangle3[3] = { Vec2(150, 125), Vec2(250, 225), Vec2(225, 125) };

  // Draw triangles
  triangle(triangle1[0], triangle1[1], triangle1[2], canvas, red);
  triangle(triangle2[0], triangle2[1], triangle2[2], canvas, lightBlue);
  triangle(triangle3[0], triangle3[1], triangle3[2], canvas, darkGreen);

  // Output the final image
  canvas.outputTGAImage("graphics_lab2.ppm");

  return 0;
}
