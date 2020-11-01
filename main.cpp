#include "mainwindow.h"
#include <QApplication>
#include "clientwidgets.h"
#include "SpoScriptEngine.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    SpoJSon * _scriptEngine;

    _scriptEngine = new SpoJSon();

    return a.exec();
}
