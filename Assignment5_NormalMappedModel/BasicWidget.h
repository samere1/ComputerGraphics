#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>

#include "Renderable.h"

/**
 * OpenGL widget for rendering an .obj model with textures and normals.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
  QMatrix4x4 world_;
  QMatrix4x4 model_;
  QMatrix4x4 view_;
  QMatrix4x4 projection_;

  QElapsedTimer frameTimer_;

  Renderable *renderable_;

  QOpenGLDebugLogger logger_;

  // File path for the currently displayed obj file
  std::string objFilePath_;

  // Keeps track of whether or not we're in wireframe mode
  bool wireframe = false;

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent *keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  BasicWidget(std::string objFilePath, QWidget *parent = nullptr);
  virtual ~BasicWidget();

  // Make sure we have some size that makes sense.
  QSize sizeHint() const { return QSize(800, 600); }
};
