#ifndef FILEHELPER_H
#define FILEHELPER_H
#include <QString>
class filehelper
{
public:
    filehelper();
    void handleDownloadRequest(QString title);
    void handleUploadRequest();
};

#endif // FILEHELPER_H
