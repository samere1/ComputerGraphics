#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <vector>

class ObjLoader {
public:
  // Constructor loads a filename with the .obj extension
  ObjLoader(std::string fileName);

  // Return the vertices and indices for faces
  inline std::vector<float> getVertices() { return vertices; }
  inline std::vector<float> getNormals() { return normals; }
  inline std::vector<unsigned int> getVertexIndices() { return vertexIndicies; }
  inline std::vector<unsigned int> getNormalIndices() { return normalIndicies; }
private:
  // Store the vertices and indices that make up faces
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<unsigned int> vertexIndicies;
  std::vector<unsigned int> normalIndicies;

  // Parse the vertices in the line and add them to the given vector of vertices
  void parseVertex(std::vector<float> &vertices, std::string line);
};

#endif
