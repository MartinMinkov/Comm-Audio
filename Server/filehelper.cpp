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

void filehelper::handleDownloadRequest(QString title){
    networkutility::debugMessage("download requested");
    char * fileName;
    char buff[FILEMAX] = { 0 };
    fileName = buff;
    char * ff;
    qint64 left;
    networkutility n;
    int bytesRead;
    //receiveTCP(accept_socket,fileName);
    title = title.remove(0, 1);
    qDebug() << title;
    printf("starting to open file");
    qDebug() << "starting to open file";
    /*QFile f(title);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        printf("Invalid file name");
        fflush(stdout);
        return;
    }*/
    FILE * fqt;
    fqt = fopen(title.toStdString().c_str(), "rb+");
    //ifstream fs;
    //fs.open(title.toStdString().c_str(), ios::binary);
    int total = 0;
    fflush(stdout);
    while((bytesRead = fread(buff, sizeof(char), FILEMAX, fqt))){
    //while(!fs.eof()){
      //  fs.read(buff,FILEMAX);
    //while(!f.atEnd()){
        //q = f.read(FILEMAX);
        //bytesRead = q.size();
        //ff = q.data();
    //left = f.read(buff, FILEMAX);
       //bytesRead =  fs.gcount();
        total += bytesRead;
        printf("Sending some data ok %d", total);
        fflush(stdout);
        if(bytesRead != FILEMAX){
            n.WSAS(accept_socket, buff, bytesRead, 1000);
            break;
        }
        n.WSAS(accept_socket, buff, 20000, 1000);
    }
}

void filehelper::handleUploadRequest(){
    networkutility::debugMessage("upload requested");
}

