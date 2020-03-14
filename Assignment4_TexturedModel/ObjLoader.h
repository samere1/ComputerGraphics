#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <vector>
#include <QtCore>
#include <QVector3D>
#include <QVector2D>

class ObjLoader {
public:
  // Constructor loads a filename with the .obj extension
  ObjLoader(std::string fileName);
  virtual ~ObjLoader();

  // Return the unique vertices and indices for faces
  inline QVector<QVector3D> getPositions() { return positions; }
  inline QVector<QVector2D> getTextures() { return textures; }
  inline QVector<QVector3D> getNormals() { return normals; }
  inline QVector<unsigned int> getIndices() { return indices; }
  
  inline QString getTextureFile() { return textureFile; }
private:
  // Store the vertices that make up faces
  QVector<QVector3D> objPositions;
  QVector<QVector2D> objTextures;
  QVector<QVector3D> objNormals;

  // Store unique combinations of vertices and their corresponding indices
  QVector<QVector3D> positions;
  QVector<QVector2D> textures;
  QVector<QVector3D> normals;
  QVector<unsigned int> indices;

  // Texture file from the obj's mtl file
  QString textureFile;

  // Parse the faces to get unique vertices
  void parseFaces(QMap<QVector<float>, unsigned int> verticesToIndices, QStringList faces);

  // Parses the obj's mtl file for the texture file
  void parseMtlFile(std::string mtlFileName);
};

#endif
