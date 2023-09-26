#include "Experiment1.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Experiment1 w;
    w.setWindowTitle("词法分析器");
    w.show();
    return a.exec();
}
