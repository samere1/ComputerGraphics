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
  QAction *exit = file->addAction("Quit", [this]() {close();});

  // Have the widget render the obj file with the given file path
  BasicWidget *widget = new BasicWidget(objFilePath, this);
  setCentralWidget(widget);
}
