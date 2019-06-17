#ifndef BROADCAST_H
#define BROADCAST_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "../src/const.h"

class Broadcast : public QObject
{
    Q_OBJECT
public:
    explicit Broadcast(QObject *parent = nullptr);

signals:
public slots:
    void startBroadcasting();

private slots:
    void broadcastDatagram();
private:
    QString createDatagram();

private:
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;
    int messageNo = 1;
    int mBCInterval = 3000; //broadcast interval 3 sec
    QStringList mIPs; //local ip address list
    QString mHash=""; // hash to record setting change
    QString oldTime="";
    QString mHostname="";
};

#endif // BROADCAST_H
