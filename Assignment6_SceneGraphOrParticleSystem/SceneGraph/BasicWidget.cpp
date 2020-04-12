#include "BasicWidget.h"
#include "Sphere.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget *parent) : QOpenGLWidget(parent), logger_(this)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
  delete solarSystem_;
}

//////////////////////////////////////////////////////////////////////
// Privates

// [NOTE: Methods not included in header file due to odd compiling error when trying to import Sphere.h in BasicWidget.h]

// Creates a scene node for a sphere
SceneNode *createSphereNode(Sphere sphere, QString textureFile, SceneNode *parent, float rotationSpeedChange, float xOffset, float scale) {
  // Initialize the node with the given sphere object and rotation speed
  Renderable *renderable_ = new Renderable();
  renderable_->init(sphere.positions(), sphere.normals(), sphere.texCoords(), sphere.indexes(), textureFile);
  SceneNode *node = new SceneNode();
  node->init(renderable_, rotationSpeedChange); // How much faster the object spins about its y-axis

  // Translate and scale the node
  QMatrix4x4 transform;
  transform.translate(QVector3D(xOffset, 0.0f, 0.0f));
  transform.scale(scale);
  node->setLocalTransform(transform);

  // Connect the node to its parent
  parent->addChild(node);

  return node;
}

// Creates a scene node for a sphere's orbit
SceneNode *createOrbitNode(float rotationSpeedChange, SceneNode *parent) {
  SceneNode *orbitNode = new SceneNode();
  orbitNode->init(NULL, rotationSpeedChange); // How much faster this orbit's children rotate around this orbit's parent
  parent->addChild(orbitNode);
  return orbitNode;
}


///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
  // Handle key events. We call update after we handle a key press
  // to trigger a redraw.
  if (keyEvent->key() == Qt::Key_Q) {
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

  solarSystem_ = new SceneNode();
  Sphere sphere = Sphere();

  // Sun at the center of the solar system
  SceneNode *sunNode = createSphereNode(sphere, sunTexture, solarSystem_, 2.0f, 0.0f, 1.0f);

  // Mercury rotates around the sun
  SceneNode *mercuryOrbitNode = createOrbitNode(4.0f, sunNode);
  SceneNode *mercuryNode = createSphereNode(sphere, mercuryTexture, mercuryOrbitNode, 10.0f, 2.0f, 0.25f);
  // Moon rotates around mercury
  SceneNode *mercuryMoonOrbitNode = createOrbitNode(20.0f, mercuryNode);
  createSphereNode(sphere, moonTexture, mercuryMoonOrbitNode, 40.0f, 2.0f, 0.5f);

  // Earth rotates around the sun
  SceneNode *earthOrbitNode = createOrbitNode(3.0f, sunNode);
  SceneNode *earthNode = createSphereNode(sphere, earthTexture, earthOrbitNode, 5.0f, 6.0f, 0.5f);
  // 2 moons rotate around the earth
  SceneNode *earthMoonOrbitNode1 = createOrbitNode(10.0f, earthNode);
  createSphereNode(sphere, moonTexture, earthMoonOrbitNode1, 20.0f, 1.5f, 0.25f);
  SceneNode *earthMoonOrbitNode2 = createOrbitNode(5.0f, earthNode);
  createSphereNode(sphere, moonTexture, earthMoonOrbitNode2, 10.0f, 3.0f, 0.5f);

  // Planet rotates around the sun
  SceneNode *planetOrbitNode = createOrbitNode(1.0f, sunNode);
  SceneNode *planetNode = createSphereNode(sphere, planetTexture, planetOrbitNode, 2.5f, 12.0f, 0.7f);
  // 3 moons rotate around the planet
  SceneNode *planetMoonOrbitNode1 = createOrbitNode(10.0f, planetNode);
  createSphereNode(sphere, moonTexture, planetMoonOrbitNode1, 40.0f, 1.5f, 0.2f);
  SceneNode *planetMoonOrbitNode2 = createOrbitNode(5.0f, planetNode);
  createSphereNode(sphere, moonTexture, planetMoonOrbitNode2, 20.0f, 2.5f, 0.3f);
  SceneNode *planetMoonOrbitNode3 = createOrbitNode(2.5f, planetNode);
  createSphereNode(sphere, moonTexture, planetMoonOrbitNode3, 10.0f, 3.5f, 0.4f);

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
  glViewport(0, 0, w, h);
  view_.setToIdentity();
  view_.lookAt(
    QVector3D(0.0f, 2.0f, 16.0f),
    QVector3D(0.0f, 0.0f, 0.0f),
    QVector3D(0.0f, 1.0f, 0.0f));
  projection_.setToIdentity();
  projection_.perspective(90.f, (float)w / (float)h, 0.001, 1000.0);
  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  qint64 msSinceRestart = frameTimer_.restart();
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  solarSystem_->update(msSinceRestart);
  solarSystem_->draw(view_, projection_);
  update();
}
