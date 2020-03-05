#include "Application.h"
#include "BasicWidget.h"

Application::Application(QWidget *parent) : QMainWindow(parent)
{}

Application::~Application()
{}

void Application::buildGui(std::string objFilePath)
{
  // A simple menubar
  QMenuBar *menu = menuBar();

  QMenu *file = menu->addMenu("File");

  objFilePath_ = objFilePath;
  //QAction *open = file->addAction("Open", [this]() {
  //  objFilePath_ = QFileDialog::getOpenFileName(this, tr("Open OBJ file"), "", tr("OBJ Files (*.obj)"), 0, QFileDialog::DontUseNativeDialog).toStdString();
  //  }
  //);
  
  file->addSeparator();
  QAction *exit = file->addAction("Quit", [this]() {close();});

  // Our basic widget
  BasicWidget *widget = new BasicWidget(objFilePath_, this);
  setCentralWidget(widget);
}
