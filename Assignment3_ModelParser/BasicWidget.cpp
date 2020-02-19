#include "BasicWidget.h"
#include "ObjLoader.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget *parent)
  : QOpenGLWidget(parent),
  vboBunny_(QOpenGLBuffer::VertexBuffer), iboBunny_(QOpenGLBuffer::IndexBuffer),
  vboMonkey_(QOpenGLBuffer::VertexBuffer), iboMonkey_(QOpenGLBuffer::IndexBuffer),
  vboCube_(QOpenGLBuffer::VertexBuffer), iboCube_(QOpenGLBuffer::IndexBuffer)
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
  vboBunny_.release(); vboBunny_.destroy();
  iboBunny_.release(); iboBunny_.destroy();
  vboMonkey_.release(); vboMonkey_.destroy();
  iboMonkey_.release(); iboMonkey_.destroy();
  vboCube_.release(); vboCube_.destroy();
  iboCube_.release(); iboCube_.destroy();
  vao_.release(); vao_.destroy();
}

//////////////////////////////////////////////////////////////////////
// Privates
QString BasicWidget::vertexShaderString() const
{
  return
    "#version 330\n"
    "layout(location = 0) in vec3 position;\n"

    "uniform mat4 modelMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 projectionMatrix;\n"

    "out vec4 vertColor;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);\n"
    "  vertColor = vec4(1.0, 1.0, 1.0, 1.0);\n" // Color triangles white
    "}\n";
}

QString BasicWidget::fragmentShaderString() const
{
  return
    "#version 330\n"
    "in vec4 vertColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "  color = vertColor;\n"
    "}\n";
}

void BasicWidget::createShader()
{
  QOpenGLShader vert(QOpenGLShader::Vertex);
  vert.compileSourceCode(vertexShaderString());
  QOpenGLShader frag(QOpenGLShader::Fragment);
  frag.compileSourceCode(fragmentShaderString());
  bool ok = shaderProgram_.addShader(&vert);
  if (!ok) {
    qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.addShader(&frag);
  if (!ok) {
    qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.link();
  if (!ok) {
    qDebug() << shaderProgram_.log();
  }
}

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
  else if (keyEvent->key() == Qt::Key_Z) {
    qDebug() << "Toggling zoom.";
    zoomOut = !zoomOut;
    resizeGL(width(), height());
    update();
  }
  else if (keyEvent->key() == Qt::Key_Q) {
    qDebug() << "Exiting program.";
    exit(0);
  }
  else if (keyEvent->key() == Qt::Key_1) {
    qDebug() << "Rendering bunny.";
    currentObj = 0;
    update();
  }
  else if (keyEvent->key() == Qt::Key_2) {
    qDebug() << "Rendering monkey.";
    currentObj = 1;
    update();
  }
  else if (keyEvent->key() == Qt::Key_3) {
    // To get a better view of the cube, press "z" to zoom out
    qDebug() << "Rendering cube.";
    currentObj = 2;
    update();
  }
  else {
    qDebug() << "You pressed an unsupported key.";
  }
}
void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  QOpenGLContext *curContext = this->context();
  qDebug() << "[BasicWidget]::initializeGL() -- Context Information:";
  qDebug() << "  Context Valid: " << std::string(curContext->isValid() ? "true" : "false").c_str();
  qDebug() << "  GL Version Used: " << curContext->format().majorVersion() << "." << curContext->format().minorVersion();
  qDebug() << "  Vendor: " << reinterpret_cast<const char *>(glGetString(GL_VENDOR));
  qDebug() << "  Renderer: " << reinterpret_cast<const char *>(glGetString(GL_RENDERER));
  qDebug() << "  Version: " << reinterpret_cast<const char *>(glGetString(GL_VERSION));
  qDebug() << "  GLSL Version: " << reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

  // Set up our shaders
  createShader();

  // Temporary bind of our shader
  shaderProgram_.bind();

  // Initialize vertex and index buffers for bunny, monkey, and cube
  initializeBuffers(bunnyObj, vboBunny_, iboBunny_);
  initializeBuffers(monkeyObj, vboMonkey_, iboMonkey_);
  initializeBuffers(cubeObj, vboCube_, iboCube_);

  // Create a VAO to keep track of things for us
  vao_.create();
  setBuffers();

  glViewport(0, 0, width(), height());
}

// Use the object to initialize the buffers
void BasicWidget::initializeBuffers(ObjLoader obj, QOpenGLBuffer &vbo_, QOpenGLBuffer &ibo_) {
  std::vector<float> verts = obj.getVertices();
  vbo_.create();
  vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vbo_.bind();
  vbo_.allocate(&verts[0], verts.size() * sizeof(GL_FLOAT));
  vbo_.release();

  std::vector<unsigned int> idx = obj.getVertexIndices();
  ibo_.create();
  ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  ibo_.bind();
  ibo_.allocate(&idx[0], idx.size() * sizeof(GL_UNSIGNED_INT));
  ibo_.release();
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);

  // View and projection matrices for zooming out to get a better view of the objects
  if (zoomOut) {
    view_.lookAt(QVector3D(2, 3, 2), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    projection_.perspective(50.0f, (float)w / (float)h, 0.1f, 100.0f);
  }
  else {
    view_.setToIdentity();
    projection_.setToIdentity();
  }

  shaderProgram_.bind();
  shaderProgram_.setUniformValue("viewMatrix", view_);
  shaderProgram_.setUniformValue("projectionMatrix", projection_);
}

void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Only fill in triangles if not in wireframe mode
  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

  // Update buffers and get number of vertices to draw
  int numVertices = setBuffers();
  glDrawElements(GL_TRIANGLES, numVertices * 3, GL_UNSIGNED_INT, 0);

  vao_.release();
  shaderProgram_.release();
}

// Sets the vertex and index buffers for the current object.
// Returns number of vertices to render.
int BasicWidget::setBuffers() {
  // Get the vertices and indices of the current object to render
  QOpenGLBuffer vbo_, ibo_;
  int numVertices;
  switch (currentObj) {
  case 0:
    vbo_ = vboBunny_;
    ibo_ = iboBunny_;
    numVertices = bunnyObj.getVertexIndices().size();
    break;
  case 1:
    vbo_ = vboMonkey_;
    ibo_ = iboMonkey_;
    numVertices = monkeyObj.getVertexIndices().size();
    break;
  case 2:
    vbo_ = vboCube_;
    ibo_ = iboCube_;
    numVertices = cubeObj.getVertexIndices().size();
    break;
  }

  shaderProgram_.bind();
  vao_.bind();
  vbo_.bind();
  ibo_.bind();

  // Enable the attribute array for the vertices and draw them
  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3);

  return numVertices;
}
