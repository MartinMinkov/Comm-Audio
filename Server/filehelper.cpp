#include "filehelper.h"
#include "networkutility.h"
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QDebug>
#define FILEMAX 10000
filehelper::filehelper()
{

}

void filehelper::handleDownloadRequest(QString title){
    networkutility::debugMessage("download requested");
    char * fileName;
    char buff[FILEMAX] = { 0 };
    fileName = buff;
    qint64 left;
    networkutility n;
    //n.receiveTCP(accept_socket,fileName);
    title = title.remove(0, 1);
    qDebug() << title;
    printf("starting to open file");
    qDebug() << "starting to open file";
    QFile f(title);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        printf("Invalid file name");
        fflush(stdout);
        return;
    }
    printf("File opened ?");
    fflush(stdout);
    while(!f.atEnd()){
        left = f.read(buff, FILEMAX);
        printf("Sending some data ok");
        fflush(stdout);
        n.WSAS(accept_socket, buff, 1000, 1000);
    }
}

void filehelper::handleUploadRequest(){
    networkutility::debugMessage("upload requested");
}

