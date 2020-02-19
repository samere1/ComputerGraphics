#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>

#include "ObjLoader.h"

/**
 * OpenGL widget for rendering some .obj models in wireframe mode.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
  QString vertexShaderString() const;
  QString fragmentShaderString() const;
  void createShader();

  // Use the object to initialize the buffers
  void initializeBuffers(ObjLoader obj, QOpenGLBuffer &vbo_, QOpenGLBuffer &ibo_);
  // Sets the vertex and index buffers for the current object.
  // Returns number of vertices to render.
  int setBuffers();

  QOpenGLVertexArrayObject vao_;

  // Load obj files with vertices and indices
  ObjLoader bunnyObj = ObjLoader("./objects/bunny_centered.obj");
  ObjLoader monkeyObj = ObjLoader("./objects/monkey_centered.obj");
  ObjLoader cubeObj = ObjLoader("./objects/cube.obj");
  int currentObj = 0; // 0 = bunny, 1 = monkey, 2 = cube

  // Keeps track of whether or not we're in wireframe mode
  bool wireframe = true;

  // View and projection matrices for zooming out to get a better view of the objects
  QMatrix4x4 view_;
  QMatrix4x4 projection_;
  bool zoomOut = false;

protected:
  // Required overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  QOpenGLShaderProgram shaderProgram_;

  // Vertex and index buffers for the objects
  QOpenGLBuffer vboBunny_, iboBunny_;
  QOpenGLBuffer vboMonkey_, iboMonkey_;
  QOpenGLBuffer vboCube_, iboCube_;

public:
  BasicWidget(QWidget* parent = nullptr);
  virtual ~BasicWidget();

  // Make sure we have some size that makes sense.
  QSize sizeHint() const { return QSize(800, 600); }
};
