#include "server.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":qdarkstyle/style.qss");
    styleFile.open(QFile::ReadOnly | QFile::Text);
    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    server w;
    w.show();

    return a.exec();
}
