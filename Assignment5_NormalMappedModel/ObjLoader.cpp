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
      parseFace(verticesToIndices, tokens);
    }
    // Parse mtl file to get texture and normal map paths
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

// Parse the face to get unique vertices
void ObjLoader::parseFace(QMap<QVector<float>, unsigned int> verticesToIndices, QStringList verticesStrings) {
  // Vertices of the triangle face
  QVector<QVector<float>> vertices;
  // Saving data to calculate tangent (could get this data from vertices array, but this is more convenient)
  QVector<QVector3D> verticesPositions;
  QVector<QVector2D> verticesTextures;

  // Parse the face data into vertices
  for (int i = 1; i < verticesStrings.size(); i++) {
    QStringList vertexIndices = verticesStrings.at(i).split('/');

    // Create the vertex
    QVector<float> vertex;
    QVector3D position = objPositions[vertexIndices[0].toUInt() - 1];
    QVector2D texture = vertexIndices[1] != "" ? objTextures[vertexIndices[1].toUInt() - 1] : QVector2D(0, 0); // Texture coord optional
    QVector3D normal = objNormals[vertexIndices[2].toUInt() - 1];
    vertex << position.x() << position.y() << position.z();
    vertex << texture.x() << texture.y();
    vertex << normal.x() << normal.y() << normal.z();

    // Save all three vertices then index all of them afterwards
    vertices << vertex;
    // Saving data to calculate tangent (could get this data from vertices array, but this is more convenient)
    verticesPositions << position;
    verticesTextures << texture;
  }

  // Calculate position and texture deltas
  QVector3D deltaPos1 = verticesPositions.at(1) - verticesPositions.at(0);
  QVector3D deltaPos2 = verticesPositions.at(2) - verticesPositions.at(0);
  QVector2D deltaUV1 = verticesTextures.at(1) - verticesTextures.at(0);
  QVector2D deltaUV2 = verticesTextures.at(2) - verticesTextures.at(0);

  // Calculate the tangent (bitangent calculated later in the vertex shader)
  float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
  QVector3D tangent = (deltaPos1 * deltaUV2.y() - deltaPos2 * deltaUV1.y()) * r;
  tangents << tangent << tangent << tangent;

  // Indexing
  for (int i = 0; i < vertices.size(); i++) {
    QVector<float> vertex = vertices.at(i);
    // If a vertex already exists, use the existing index
    unsigned int index = verticesToIndices.value(vertex, -1);
    // If the vertex is new, add it to the arrays and the map with a new index
    if (index == -1) {
      positions.append(QVector3D(vertex.at(0), vertex.at(1), vertex.at(2)));
      textures.append(QVector2D(vertex.at(3), vertex.at(4)));
      normals.append(QVector3D(vertex.at(5), vertex.at(6), vertex.at(7)));

      index = positions.size() - 1;
      verticesToIndices.insert(vertex, index);
    }
    else {
      // If we found a similar vertex, average the tangents
      tangents[index] = (tangents[index] + tangents[i]) / 2;
    }
    indices.append(index);
  }
}

// Parses the obj's mtl file for the texture and normal map paths
void ObjLoader::parseMtlFile(std::string mtlFileName) {
  std::ifstream inFile;
  inFile.open(mtlFileName);
  if (!inFile.is_open()) {
    std::cout << "Unable to open file " << mtlFileName << std::endl;
    exit(1);
  }

  // Parse mtl file for map paths
  std::string mtlLine;
  while (getline(inFile, mtlLine)) {
    QStringList tokens = QString::fromStdString(mtlLine).split(' ');
    // Texture map
    if (tokens[0] == "map_Kd") {
      textureFile = getFilePath(QString::fromStdString(mtlFileName), tokens[1]);
    }
    // Normal map
    else if (tokens[0] == "map_Bump") {
      normalFile = getFilePath(QString::fromStdString(mtlFileName), tokens[1]);
    }
  }
  inFile.close();
}

// Returns the full file path of the given file name using the mtl file's path
QString ObjLoader::getFilePath(QString mtlFileName, QString fileName) {
  return QFileInfo(mtlFileName).absoluteDir().absoluteFilePath(fileName);
}