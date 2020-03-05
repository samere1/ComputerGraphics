#include "BasicWidget.h"
#include "ObjLoader.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(std::string objFilePath, QWidget *parent) : QOpenGLWidget(parent), logger_(this)
{
  setFocusPolicy(Qt::StrongFocus);
  objFilePath_ = objFilePath;
}

BasicWidget::~BasicWidget()
{
  delete renderable_;
}

//////////////////////////////////////////////////////////////////////
// Privates

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
  // Handle key events. We call update after we handle a key press
  // to trigger a redraw.
  if (keyEvent->key() == Qt::Key_W) {
    qDebug() << "Toggling wireframe.";
    wireframe = !wireframe;
    update();
  }
  else if (keyEvent->key() == Qt::Key_Q) {
    qDebug() << "Exiting program.";
    exit(0);
  }
  else {
    qDebug() << "You pressed an unsupported key.";
  }
}
void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  // Get the object's verticies, indices, and texture file
  ObjLoader obj = ObjLoader(objFilePath_);
  QVector<QVector3D> pos = obj.getPositions();
  QVector<QVector3D> norm = obj.getNormals();
  QVector<QVector2D> texCoord = obj.getTextures();
  QVector<unsigned int> idx = obj.getIndices();
  QString texFile = obj.getTextureFile();

  renderable_ = new Renderable();
  renderable_->init(pos, norm, texCoord, idx, texFile);
  renderable_->setRotationAxis(QVector3D(0, 1, 0));

  glViewport(0, 0, width(), height());
  frameTimer_.start();
}

void BasicWidget::resizeGL(int w, int h)
{
  if (!logger_.isLogging()) {
    logger_.initialize();
    // Setup the logger for real-time messaging
    connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=]() {
      const QList<QOpenGLDebugMessage> messages = logger_.loggedMessages();
      for (auto msg : messages) { qDebug() << msg; } });
    logger_.startLogging();
  }
  view_.setToIdentity();
  view_.lookAt(
    QVector3D(0.0f, 0.0f, 4.0f),
    QVector3D(0.0f, 0.0f, 0.0f),
    QVector3D(0.0f, 1.0f, 0.0f));
  projection_.setToIdentity();
  projection_.perspective(70.f, (float)w / (float)h, 0.001, 1000.0);
  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  qint64 msSinceRestart = frameTimer_.restart();
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderable_->update(msSinceRestart);
  renderable_->draw(view_, projection_, wireframe);
  update();
}
