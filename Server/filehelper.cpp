#include "filehelper.h"
#include "networkutility.h"

filehelper::filehelper()
{

}

void filehelper::handleDownloadRequest(){
    networkutility::debugMessage("download requested");
}

void filehelper::handleUploadRequest(){
    networkutility::debugMessage("upload requested");
}

