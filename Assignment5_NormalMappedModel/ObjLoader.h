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
  inline QVector<QVector3D> getTangents() { return tangents; }
  inline QVector<unsigned int> getIndices() { return indices; }
  
  // Return the texture and normal maps
  inline QString getTextureFile() { return textureFile; }
  inline QString getNormalFile() { return normalFile; }
private:
  // Store the vertices that make up faces
  QVector<QVector3D> objPositions;
  QVector<QVector2D> objTextures;
  QVector<QVector3D> objNormals;

  // Store unique combinations of vertices and their corresponding indices
  QVector<QVector3D> positions;
  QVector<QVector2D> textures;
  QVector<QVector3D> normals;
  QVector<QVector3D> tangents;
  QVector<unsigned int> indices;

  // Files from the obj's mtl file
  QString textureFile;
  QString normalFile;

  // Parse the face to get unique vertices
  void parseFace(QMap<QVector<float>, unsigned int> verticesToIndices, QStringList verticesStrings);

  // Parses the obj's mtl file for the texture and normal map paths
  void parseMtlFile(std::string mtlFileName);

  // Returns the full file path of the given file name using the mtl file's path
  QString getFilePath(QString mtlFileName, QString fileName);
};

#endif
