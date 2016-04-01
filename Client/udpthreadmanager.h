#ifndef UDPTHREADMANAGER_H
#define UDPTHREADMANAGER_H

#include <QObject>

class UDPThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit UDPThreadManager(QObject *parent = 0);

signals:

public slots:
};

#endif // UDPTHREADMANAGER_H