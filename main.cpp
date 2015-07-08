#include <QApplication>
#include <gui/tMainForm.h>


int main(int argc, char *argv[])
{

  tMainForm *MainForm;
  QApplication app(argc, argv);
  MainForm = new tMainForm;
  MainForm->show();

  return app.exec();
}
