/**
 * Support code written by Erik W. Anderson
 */

#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#include <iostream>

#include "Application.h"


int main(int argc, char **argv) {
  QApplication a(argc, argv);
  QString appDir = a.applicationDirPath();
  QDir::setCurrent(appDir);

  // Check if an argument was passed in
  if (argc < 2) {
    std::cout << "Please pass in the .obj file as an argument" << std::endl;
  }

  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  fmt.setStencilBufferSize(8);
  fmt.setVersion(3, 3);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(fmt);

  Application app;
  app.buildGui(std::string(argv[1]));
  app.show();
  return QApplication::exec();
}
