/**
 * The primary application code.
 */

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

class Application : public QMainWindow
{
  Q_OBJECT

public:
  // Build the GUI with the given obj file
  void buildGui(std::string objFilePath);
  
  Application(QWidget *parent = 0);
  virtual ~Application();

signals:

public slots:

private:
  // File path for the currently displayed obj file
  std::string objFilePath_;
};
