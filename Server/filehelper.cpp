#include "filehelper.h"

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

    int index = 0;
    //find the corresponding song title with filepath
    for(QStringList::iterator it = playlist.begin(); it != playlist.end(); ++it, index++){
        QString current = *it;
        if(QString::compare(current,title,Qt::CaseSensitive) == 0){
            title = playlistWithPath.at(index);
            break;
        }
    }

    if(!(fqt = fopen(title.toStdString().c_str(), "rb+"))){
        sendDataTCP(m_socket, ERROR_BIT);
        return;
    }

    char success[] = "M";
    sendDataTCP(m_socket, success);
    while((bytesRead = fread(buff, sizeof(char), FILEMAX, fqt))){
            if(bytesRead != FILEMAX){
            n.WSAS(m_socket, buff, bytesRead, 1000);
            break;
        }
        n.WSAS(m_socket, buff, 60000, 1000);
    }
}

void filehelper::handleUploadRequest(){
    networkutility::debugMessage("upload requested");
}

