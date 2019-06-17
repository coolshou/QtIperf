#include "broadcast.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QHostInfo>

#include <QDebug>

//broadcast endpoint's info in Json format
Broadcast::Broadcast(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    //TODO: bind to
    //udpSocket->bind(QHostAddress::LocalHost, 7755);
    //connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    connect(&timer, &QTimer::timeout, this, &Broadcast::broadcastDatagram);
    //get ip address
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        //TODO: IPv6
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            mIPs.append(address.toString());
    }
    //get host name
    mHostname = QHostInfo::localHostName();

}
QString Broadcast::createDatagram(){
    QString strBuffer;
    QDateTime time;
    QJsonArray jArrIPs;
    foreach (QString ip, mIPs) {
       jArrIPs.append(ip);
    }
    QJsonObject jObj;
    //jObj.insert("SN", messageNo);
    jObj.insert("HOSTNAME", mHostname);
    jObj.insert("IP", jArrIPs);
    jObj.insert("PORT", JCONRPC_PORT); //xml rpc port

    QJsonDocument jDoc;
    jDoc.setObject(jObj);
    //
    QString hash = QString(QCryptographicHash::hash((jDoc.toJson()),
                                                    QCryptographicHash::Md5).toHex());
    if (hash!=mHash){
        mHash=hash;
        time = QDateTime::currentDateTime();
        strBuffer = time.toString("yyyy-MM-dd hh:mm:ss");
        oldTime=strBuffer;
    }else{
        strBuffer=oldTime;
    }
    jObj.insert("TIMESTEMP", strBuffer);
    jDoc.setObject(jObj);
    //TODO: compress first, encode/decode?
    return jDoc.toJson();
}
void Broadcast::startBroadcasting()
{
    timer.start(mBCInterval);
}

void Broadcast::broadcastDatagram()
{
    QString s;
    s= createDatagram();

    QByteArray datagram = s.toUtf8();
    udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, IPERF_BCAST_PORT);

    ++messageNo;
}
