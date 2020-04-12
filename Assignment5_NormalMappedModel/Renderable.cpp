#include "Renderable.h"

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), normal_(QOpenGLTexture::Target2D), numIndices_(0), vertexSize_(0), rotationAxis_(0.0, 0.0, 1.0), rotationSpeed_(0.05)
{
  rotationAngle_ = 0.0;
}

Renderable::~Renderable()
{
  if (texture_.isCreated()) {
    texture_.destroy();
  }
  if (normal_.isCreated()) {
    normal_.destroy();
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

void Renderable::init(const QVector<QVector3D> &positions, const QVector<QVector3D> &normals, const QVector<QVector2D> &texCoords, const QVector<QVector3D> &tangents, const QVector<unsigned int> &indexes, const QString &textureFile, const QString &normalFile)
{
  // Need to make sure our sizes all work out ok
  if (positions.size() != texCoords.size() || positions.size() != normals.size() || positions.size() != tangents.size()) {
    qDebug() << "[Renderable]::init() -- positions size mismatch with normals/textureCoords/tangents";
    return;
  }

  // Only load objects with texture and normal maps
  if (textureFile == "" || normalFile == "") {
    qDebug() << "[Renderable]::init() -- object does not have a texture or normal map";
    return;
  }
  texture_.setData(QImage(textureFile).mirrored(true));
  normal_.setData(QImage(normalFile).mirrored(true));

  // Set our model matrix to identity
  modelMatrix_.setToIdentity();

  // Set our number of indices
  numIndices_ = indexes.size();

  // Calculate number of verts to size our vbo
  int numVerts = positions.size();
  vertexSize_ = 3 + 3 + 2 + 3;  // Position + normal + tex coords + tangent
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
    data[(i * vertexSize_) + 3] = normals.at(i).x();
    data[(i * vertexSize_) + 4] = normals.at(i).y();
    data[(i * vertexSize_) + 5] = normals.at(i).z();
    data[(i * vertexSize_) + 6] = texCoords.at(i).x();
    data[(i * vertexSize_) + 7] = texCoords.at(i).y();
    data[(i * vertexSize_) + 8] = tangents.at(i).x();
    data[(i * vertexSize_) + 9] = tangents.at(i).y();
    data[(i * vertexSize_) + 10] = tangents.at(i).z();
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
  shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, vertexSize_ * sizeof(float));
  shader_.enableAttributeArray(2);
  shader_.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(float), 2, vertexSize_ * sizeof(float));
  shader_.enableAttributeArray(3);
  shader_.setAttributeBuffer(3, GL_FLOAT, 8 * sizeof(float), 3, vertexSize_ * sizeof(float));

  // Release our vao and THEN release our buffers.
  vao_.release();
  vbo_.release();
  ibo_.release();
}

void Renderable::update(const qint64 msSinceLastFrame)
{
  // Want our polygon to rotate
  float sec = msSinceLastFrame / 1000.0f;
  float anglePart = sec * rotationSpeed_ * 360.f;
  rotationAngle_ += anglePart;
  while (rotationAngle_ >= 360.0) {
    rotationAngle_ -= 360.0;
  }
  
  // Light hitting the object
  shader_.bind();
  shader_.setUniformValue("pointLight.position", 3.0f, 3.0f, 6.0f);
  shader_.setUniformValue("pointLight.color", 1.0f, 1.0f, 1.0f);
  shader_.setUniformValue("pointLight.ambientIntensity", 0.5f);
  shader_.setUniformValue("pointLight.specularStrength", 0.5f);
  shader_.setUniformValue("pointLight.constant", 1.0f);
  shader_.setUniformValue("pointLight.linear", 0.09f);
  shader_.setUniformValue("pointLight.quadratic", 0.032f);
  shader_.release();
}

void Renderable::draw(const QMatrix4x4 &world, const QMatrix4x4 &view, const QMatrix4x4 &projection, bool wireframe)
{
  // Create our model matrix
  QMatrix4x4 rotMatrix;
  rotMatrix.setToIdentity();
  rotMatrix.rotate(rotationAngle_, rotationAxis_);

  QMatrix4x4 modelMat = modelMatrix_ * rotMatrix;
  modelMat = world * modelMat;
  // Make sure our state is what we want
  shader_.bind();
  // Set our matrix uniforms
  QMatrix4x4 id;
  id.setToIdentity();
  shader_.setUniformValue("modelMatrix", modelMat);
  shader_.setUniformValue("viewMatrix", view);
  shader_.setUniformValue("projectionMatrix", projection);
  shader_.setUniformValue("textureExists", texture_.isCreated());
  
  // Set the texture and normal maps
  vao_.bind();
  QOpenGLFunctions f(QOpenGLContext::currentContext());
  f.glActiveTexture(GL_TEXTURE0);
  texture_.bind();
  shader_.setUniformValue("diffuseMap", GL_TEXTURE0);
  f.glActiveTexture(GL_TEXTURE1);
  normal_.bind();
  shader_.setUniformValue("normalMap", GL_TEXTURE1 - GL_TEXTURE0);
  
  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
  glDrawElements(GL_TRIANGLES, numIndices_, GL_UNSIGNED_INT, 0);

  texture_.release();
  normal_.release();
  vao_.release();
  shader_.release();
}

void Renderable::setModelMatrix(const QMatrix4x4 &transform)
{
  modelMatrix_ = transform;
}

void Renderable::setRotationAxis(const QVector3D &axis)
{
  rotationAxis_ = axis;
}

void Renderable::setRotationSpeed(float speed)
{
  rotationSpeed_ = speed;
}