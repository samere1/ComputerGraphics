#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "ObjLoader.h"

ObjLoader::ObjLoader(std::string fileName) {
  // Only open files with .obj extension
  std::size_t extIndex = fileName.rfind('.');
  if ((extIndex != std::string::npos) && (fileName.substr(extIndex) != ".obj")) {
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
  QMap<QVector<float>, unsigned int> verticesToIndices;
  std::string line;
  while (getline(inFile, line)) {
    QStringList tokens = QString::fromStdString(line).split(' ');
    // Parse vertex data
    if (tokens[0] == "v") {
      objPositions.append(QVector3D(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()));
    }
    // Parse vertex texture data
    else if (tokens[0] == "vt") {
      objTextures.append(QVector2D(tokens[1].toFloat(), tokens[2].toFloat()));
    }
    // Parse vertex normal data
    else if (tokens[0] == "vn") {
      objNormals.append(QVector3D(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat()));
    }
    // Parse faces to get vertices
    else if (tokens[0] == "f") {
      parseFaces(verticesToIndices, tokens);
    }
    else if (tokens[0] == "mtllib") {
      parseMtlFile(QFileInfo(QString::fromStdString(fileName)).absoluteDir().absoluteFilePath(tokens[1]).toStdString());
    }
  }
  verticesToIndices.clear();
  inFile.close();
}

ObjLoader::~ObjLoader() {
  objPositions.clear();
  objTextures.clear();
  objNormals.clear();
  positions.clear();
  textures.clear();
  normals.clear();
  indices.clear();
}

// Parse the faces to get unique vertices
void ObjLoader::parseFaces(QMap<QVector<float>, unsigned int> verticesToIndices, QStringList faces) {
  for (int i = 1; i < faces.size(); i++) {
    QString faceString = faces.at(i);
    QStringList vertexIndices = faceString.split('/');

    // Create the vertex
    QVector<float> face;
    QVector3D position = objPositions[vertexIndices[0].toUInt() - 1];
    QVector2D texture = vertexIndices[1] != "" ? objTextures[vertexIndices[1].toUInt() - 1] : QVector2D(0, 0); // Texture coord optional
    QVector3D normal = objNormals[vertexIndices[2].toUInt() - 1];
    face << position.x() << position.y() << position.z();
    face << texture.x() << texture.y();
    face << normal.x() << normal.y() << normal.z();

    // If the vertex already exists, use the existing index
    unsigned int index = verticesToIndices.value(face, -1);
    // If the vertex is new, add it to the arrays and the map with a new index
    if (index == -1) {
      positions.append(position);
      textures.append(texture);
      normals.append(normal);
      
      index = positions.size() - 1;
      verticesToIndices.insert(face, index);
    }
    indices.append(index);
  }
}

// Parses the obj's mtl file for the texture file
void ObjLoader::parseMtlFile(std::string mtlFileName) {
  std::ifstream inFile;
  inFile.open(mtlFileName);
  if (!inFile.is_open()) {
    std::cout << "Unable to open file " << mtlFileName << std::endl;
    exit(1);
  }

  // Parse mtl file for texture file path on the "map_Kd" line
  std::string mtlLine;
  while (getline(inFile, mtlLine)) {
    QStringList tokens = QString::fromStdString(mtlLine).split(' ');
    if (tokens[0] == "map_Kd") {
      textureFile = QFileInfo(QString::fromStdString(mtlFileName)).absoluteDir().absoluteFilePath(tokens[1]);
      break;
    }
  }
  inFile.close();
}
