#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <QtCore>

#include "ObjLoader.h"

ObjLoader::ObjLoader(std::string fileName) {
  // Only open files with .obj extension
  if (fileName.substr(fileName.rfind('.')) != ".obj") {
    std::cout << "Please provide a file with the .obj extension" << std::endl;
    exit(1);
  }

  // Attempt to open the file
  std::ifstream inFile;
  inFile.open(fileName);
  if (!inFile.is_open()) {
    std::cout << "Unable to open file " << fileName << std::endl;
    exit(1);
  }

  // Parse the file
  std::string line;
  while (getline(inFile, line)) {
    // Parse vertex data
    if (line.substr(0, 2) == "v ") {
      parseVertex(vertices, line);
    }
    // Parse vertex normal data
    else if (line.substr(0, 3) == "vn ") {
      parseVertex(normals, line);
    }
    // Parse face for its vertex and vertex normal indices
    else if (line.substr(0, 2) == "f ") {
      QStringList vertices = QString::fromStdString(line).split(' ');
      for (int i = 1; i < vertices.size(); i++) {
        QString vertex = vertices.at(i);
        QStringList indices = vertex.split('/');
        vertexIndicies.push_back(indices[0].toUInt() - 1);
        normalIndicies.push_back(indices[2].toUInt() - 1);
      }
    }
  }

  inFile.close();
}

// Parse the vertices in the line and add them to the given vector of vertices
void ObjLoader::parseVertex(std::vector<float> &vertices, std::string line) {
  QStringList coordinates = QString::fromStdString(line).split(' ');
  vertices.push_back(coordinates[1].toFloat());
  vertices.push_back(coordinates[2].toFloat());
  vertices.push_back(coordinates[3].toFloat());
}
