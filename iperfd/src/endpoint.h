#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>
#include <QList>
#include <QThread>

#include "iperfserver.h"
#include "iperfclient.h"
#include "broadcast.h"
//#include "maiaXmlRpcServer.h"
#include "../src/iperfxmlpair.h"

class Endpoint : public QObject
{
    Q_OBJECT
public:
    explicit Endpoint(QObject *parent = nullptr);

signals:

private slots:
    void onIperfServerFinished();
    void onIperfServerStdout(QString line);
    void onIperfServerStderr(QString line);

    void onIperfClientFinished();
    void onIperfClientStdout(QString line);
    void onIperfClientStderr(QString line);

public slots:
    QString getIperfVersion();

    int getIperfServerIdx();
    int addIperfServer(QString cfg);
    bool startIperfServer(int idx);
    bool stopIperfServer(int idx);
    void startAllIperfServer();
    void stopAllIperfServer();
    bool isIperfServerRunning(int idx);

    /* int addIperfClient(QString ServerIP,int ServerPort=BASE_PORT,
                       int iMode=0, int iTime=10, int iOmit=0,
                       int iBuffer=0, int iWindow=0,
                       int iUDPbitrate=0, bool bReverse=false);*/
    int  addIperfClient(QString cfg);
    bool startIperfClient(int idx);
    bool stopIperfClient(int idx);
    void startAllIperfClient();
    void stopAllIperfClient();
    bool isIperfClientRunning(int idx);

private:
    MaiaXmlRpcServer *server;
    Broadcast *bc;
    //iperf server
    QList<QThread*> iperfServerThreads; //
    QList<IperfServer*> iperfServers; //iperf servers
    //iperf client
    QList<QThread*> iperfClientThreads; //
    QList<IperfClient*> iperfClients; //iperf clients
};

#endif // ENDPOINT_H
