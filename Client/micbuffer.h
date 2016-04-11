#ifndef MICBUFFER_H
#define MICBUFFER_H


class micBuffer : public QIODevice
{
public:
    micBuffer();
    qint64 readData(char * data, qint64 maxlen);
    qint64 writeData(const char * data, qint64 maxlen);
    qint64 bytesAvailable();
};

#endif // MICBUFFER_H
