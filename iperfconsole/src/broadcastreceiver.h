#ifndef BROADCASTRECEIVER_H
#define BROADCASTRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QMap>

class BroadcastReceiver : public QObject
{
    Q_OBJECT
public:
    explicit BroadcastReceiver(QObject *parent = nullptr);

signals:
    void updateEndpoint(QString hostname, QString json);

public slots:
private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket = nullptr;
    QMap<QString,QString> mapHostnames;
};

#endif // BROADCASTRECEIVER_H
