#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QObject>
#include <QList>
#include <QThread>

#include "iperfserver.h"
#include "iperfclient.h"
#include "broadcast.h"
#include "../src/iperfxmlpair.h"

class EndpointService : public QObject
{
    Q_OBJECT
public:
    EndpointService();
    virtual ~EndpointService();
    Q_INVOKABLE QString getIperfVersion(int notUse);
    //iperf server
    Q_INVOKABLE int getIperfServerIdx(int notUse);
    Q_INVOKABLE int addIperfServer(const QString& cfg);
    Q_INVOKABLE bool startIperfServer(int idx);
    Q_INVOKABLE bool stopIperfServer(int idx);
    Q_INVOKABLE int startAllIperfServer(int notUse);
    Q_INVOKABLE int stopAllIperfServer(int notUse);
    Q_INVOKABLE bool isIperfServerRunning(int idx);

    /* int addIperfClient(QString ServerIP,int ServerPort=BASE_PORT,
                       int iMode=0, int iTime=10, int iOmit=0,
                       int iBuffer=0, int iWindow=0,
                       int iUDPbitrate=0, bool bReverse=false);*/
    //iperf client
    Q_INVOKABLE int  addIperfClient(QString cfg);
    Q_INVOKABLE bool startIperfClient(int idx);
    Q_INVOKABLE bool stopIperfClient(int idx);
    Q_INVOKABLE int startAllIperfClient(int notUse);
    Q_INVOKABLE int stopAllIperfClient(int notUse);
    Q_INVOKABLE bool isIperfClientRunning(int idx);

private slots:
    void onIperfServerFinished();
    void onIperfServerStdout(QString line);
    void onIperfServerStderr(QString line);

    void onIperfClientFinished();
    void onIperfClientStdout(QString line);
    void onIperfClientStderr(QString line);

private:
    //MaiaXmlRpcServer *server;
    Broadcast *bc;
    //iperf server
    QList<QThread*> iperfServerThreads; //
    QList<IperfServer*> iperfServers; //iperf servers
    //iperf client
    QList<QThread*> iperfClientThreads; //
    QList<IperfClient*> iperfClients; //iperf clients
};

#endif // ENDPOINT_H
