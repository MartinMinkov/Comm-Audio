#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":qdarkstyle/style.qss");
    styleFile.open(QFile::ReadOnly | QFile::Text);
    QString style(styleFile.readAll());
    a.setStyleSheet(style);
    WSADATA data;
    int ret;
    if ((ret = WSAStartup(0x0202, &data)) != 0)
    {

        exit(1);
    }
    client w;
    w.show();

    return a.exec();
}
