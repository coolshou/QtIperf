#include "broadcastreceiver.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

#include "../src/const.h"

#include <QDebug>

BroadcastReceiver::BroadcastReceiver(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(IPERF_BCAST_PORT, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
}
void BroadcastReceiver::processPendingDatagrams()
{
    bool bUpdate=false;
    QByteArray datagram;
    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
    }
    //qDebug() << "datagram:" << tr("%1").arg(datagram.constData());
    QJsonDocument jDoc=QJsonDocument::fromJson(datagram.constData());
    //qDebug() << "processPendingDatagrams:" << jDoc.toJson();
    QJsonObject jObj = jDoc.object();
    QString hostname = jObj["HOSTNAME"].toString();
    QString timestemp = jObj["TIMESTEMP"].toString();
    if (mapHostnames.contains(hostname)){
        //host exist
        if (mapHostnames.value(hostname)==timestemp){
            //timestemp same no need to update
        }else {
            mapHostnames[hostname]=timestemp;
            bUpdate=true;
        }
    }else {
        mapHostnames.insert(hostname, timestemp);
        bUpdate=true;
    }

    if (bUpdate){
        emit updateEndpoint(hostname, jDoc.toJson());
    }
}
