#include "Renderable.h"

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), numIndices_(0), vertexSize_(0)
{}

Renderable::~Renderable()
{
  if (texture_.isCreated()) {
    texture_.destroy();
  }
  if (vbo_.isCreated()) {
    vbo_.destroy();
  }
  if (ibo_.isCreated()) {
    ibo_.destroy();
  }
  if (vao_.isCreated()) {
    vao_.destroy();
  }
}

void Renderable::createShaders()
{
  QString vertexFilename = "vert.glsl";
  bool ok = shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFilename);
  if (!ok) {
    qDebug() << shader_.log();
  }
  QString fragmentFilename = "frag.glsl";
  ok = shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFilename);
  if (!ok) {
    qDebug() << shader_.log();
  }
  ok = shader_.link();
  if (!ok) {
    qDebug() << shader_.log();
  }
}

void Renderable::init(const QVector<QVector3D> &positions, const QVector<QVector3D> &normals, const QVector<QVector2D> &texCoords, const QVector<unsigned int> &indexes, const QString &textureFile)
{
  // Need to make sure our sizes all work out ok
  if (positions.size() != texCoords.size() ||
    positions.size() != normals.size()) {
    qDebug() << "[Renderable]::init() -- positions size mismatch with normals/texture coordinates";
    return;
  }

  // Set our model matrix to identity
  modelMatrix_.setToIdentity();

  // Load the texture if there is one
  if (textureFile != "") {
    texture_.setData(QImage(textureFile).mirrored(false, true));
  }

  // Set our number of indices
  numIndices_ = indexes.size();

  // Calculate number of verts to size our vbo
  int numVerts = positions.size();
  vertexSize_ = 3 + 2;  // Position + texCoord
  int numVBOEntries = numVerts * vertexSize_;

  // Setup our shader.
  createShaders();

  // Now we can set up our buffers.
  // The VBO is created -- now we must create our VAO
  vao_.create();
  vao_.bind();
  vbo_.create();
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.bind();
  // Create a temporary data array
  float *data = new float[numVBOEntries];
  for (int i = 0; i < numVerts; ++i) {
    data[(i * vertexSize_) + 0] = positions.at(i).x();
    data[(i * vertexSize_) + 1] = positions.at(i).y();
    data[(i * vertexSize_) + 2] = positions.at(i).z();
    data[(i * vertexSize_) + 3] = texCoords.at(i).x();
    data[(i * vertexSize_) + 4] = texCoords.at(i).y();
  }
  vbo_.allocate(data, numVBOEntries * sizeof(float));
  delete[] data;

  // Create our index buffer
  ibo_.create();
  ibo_.bind();
  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  // Create a temporary array for our indexes
  unsigned int *idxAr = new unsigned int[indexes.size()];
  for (int i = 0; i < indexes.size(); ++i) {
    idxAr[i] = indexes.at(i);
  }
  ibo_.allocate(idxAr, indexes.size() * sizeof(unsigned int));
  delete[] idxAr;

  // Make sure we setup our shader inputs properly
  shader_.enableAttributeArray(0);
  shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));
  shader_.enableAttributeArray(1);
  shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, vertexSize_ * sizeof(float));

  // Release our vao and THEN release our buffers.
  vao_.release();
  vbo_.release();
  ibo_.release();
}

void Renderable::draw(const QMatrix4x4 &view, const QMatrix4x4 &projection)
{
  // Make sure our state is what we want
  shader_.bind();
  // Set our matrix uniforms
  QMatrix4x4 id;
  id.setToIdentity();
  shader_.setUniformValue("modelMatrix", modelMatrix_);
  shader_.setUniformValue("viewMatrix", view);
  shader_.setUniformValue("projectionMatrix", projection);

  vao_.bind();
  if (texture_.isCreated()) texture_.bind();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glDrawElements(GL_TRIANGLES, numIndices_, GL_UNSIGNED_INT, 0);
  if (texture_.isCreated()) texture_.release();
  vao_.release();
  shader_.release();
}

void Renderable::setModelMatrix(const QMatrix4x4 &transform)
{
  modelMatrix_ = transform;
}
