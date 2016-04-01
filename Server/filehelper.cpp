#include "filehelper.h"
#include "networkutility.h"
#include <QFile>
#include <QString>
#include <QIODevice>
#include <fstream>
#include <QDebug>
#include <iostream>
#include <QByteArray>
#define FILEMAX 20000
using namespace std;
filehelper::filehelper()
{

}

void filehelper::handleDownloadRequest(QString title, SOCKET m_socket){
    networkutility::debugMessage("download requested");
    char * fileName;
    char buff[FILEMAX] = { 0 };
    fileName = buff;
    char * ff;
    qint64 left;
    networkutility n;
    int bytesRead;
    qDebug() << title;
    FILE * fqt;
    title = title.remove(0, 1);
    if(!(fqt = fopen(title.toStdString().c_str(), "rb+"))){
        sendDataTCP(m_socket, ERROR_BIT);
        return;
    }

    char * success = "MARTINMINKOV";
    sendDataTCP(m_socket, success);





    //ifstream fs;
    //fs.open(title.toStdString().c_str(), ios::binary);
    int total = 0;
    fflush(stdout);
    while((bytesRead = fread(buff, sizeof(char), FILEMAX, fqt))){
            if(bytesRead != FILEMAX){
            n.WSAS(m_socket, buff, bytesRead, 1000);
            break;
        }
        n.WSAS(m_socket, buff, 20000, 1000);
    }
}

void filehelper::handleUploadRequest(){
    networkutility::debugMessage("upload requested");
}

