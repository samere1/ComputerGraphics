#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>

#include "SceneNode.h"

/**
 * OpenGL widget for rendering a solar system using a scene graph
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

  SceneNode *solarSystem_;

  QOpenGLDebugLogger logger_;

  QString sunTexture = "./images/sun.ppm";
  QString mercuryTexture = "./images/mercury.ppm";
  QString earthTexture = "./images/earth.ppm";
  QString planetTexture = "./images/planet.ppm";
  QString moonTexture = "./images/rock.ppm";

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent *keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

public:
  BasicWidget(QWidget *parent = nullptr);
  virtual ~BasicWidget();

  // Make sure we have some size that makes sense.
  QSize sizeHint() const { return QSize(1100, 800); }
};
