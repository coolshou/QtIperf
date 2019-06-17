#include "endpointservice.h"
#include "iperfcore.h"
#include "iperfserver.h"
#include "../src/const.h"

#include "../src/iperfxmlcfg.h"

#include <QDebug>
#include <QtGlobal>

EndpointService::EndpointService() = default;
EndpointService::~EndpointService() = default;


QString EndpointService::getIperfVersion(int notUse){
    Q_UNUSED(notUse);
    IperfCore iperf;
    QString s = iperf.version();
    //qDebug() << "getIperfVersion: " << s;
    return s;
}

//iperf server ####################################################
int EndpointService::addIperfServer(const QString& cfg) {
    Q_UNUSED(cfg);
    //qDebug() << "AddIperfServer cfg:" << cfg;
    int idx = this->getIperfServerIdx(0);

    int port = BASE_PORT + idx;
    //TODO: add iperf server
    //TODO: other args, tcp/udp...
    qDebug() << "add iperf server with port:" << port;
    IperfServer *s=new IperfServer(this, port);
    qDebug() << "iperf version:" << s->version();
    connect(s, SIGNAL(finished()), this, SLOT(onIperfServerFinished()));
    connect(s, SIGNAL(sigStdout(QString)), this, SLOT(onIperfServerStdout(QString)));
    connect(s, SIGNAL(sigStderr(QString)), this, SLOT(onIperfServerStderr(QString)));
    QThread *t=new QThread;
    s->moveToThread(t);
    connect(t, SIGNAL(started()), s, SLOT(start()));
    connect(t, SIGNAL(finished()), this, SLOT(deleteLater()));

    //iperfServers << s;
    iperfServers.append(s);
    iperfServerThreads.append(t);
    return port;
    //return "test";
}
int EndpointService::getIperfServerIdx(int notUse){
    Q_UNUSED(notUse);
    //return iperfServers.size();
    return iperfServerThreads.size();
}
int EndpointService::startAllIperfServer(int notUse){
    Q_UNUSED(notUse);
    int iMax = iperfServerThreads.count();
    if (iMax>0){
        for (int i=0; i<iMax ; i++)
        {
            startIperfServer(i);
        }
    }
    return 0;
}
int EndpointService::stopAllIperfServer(int notUse){
    Q_UNUSED(notUse);
    int iMax = iperfServerThreads.count();
    if (iMax>0){
        for (int i=0; i< iperfServerThreads.count(); i++)
        {
            stopIperfServer(i);
        }
    }
    return 0;
}
bool EndpointService::startIperfServer(int idx)
{
    int iMax=iperfServerThreads.count();
    if (idx >= iMax){
        return false;
    }
    QThread  *t = iperfServerThreads.at(idx);
    t->start();
    return true;
}
bool EndpointService::stopIperfServer(int idx){
    int iMax=iperfServers.count();
    if (idx >= iMax){
        return false;
    }
    IperfServer  *c = iperfServers.at(idx);
    c->setStop();
    //TODO: wait thread stop?
    iperfServers.removeAt(idx);
    iperfServerThreads.removeAt(idx);
    /*foreach (iperfclient *c, iperfClients) {
        c->setStop();
    }*/
    return true;
}
bool EndpointService::isIperfServerRunning(int idx){
    int iMax=iperfServers.count();
    if (idx >= iMax){
        return false;
    }
    IperfServer  *c = iperfServers.at(idx);
    return c->isRunning();
}
void EndpointService::onIperfServerFinished()
{
    qDebug() << "onIperfServerFinished";
}
void EndpointService::onIperfServerStdout(QString line)
{
    qDebug() << "IperfServer: " << line;
}
void EndpointService::onIperfServerStderr(QString line)
{
    qDebug() << "IperfServer err: " << line;
}

//iperf client ####################################################
/*int  Endpoint::addIperfClient(QString ServerIP,int ServerPort,
                              int iMode, int iTime, int iOmit,
                              int iBuffer, int iWindow,
                              int iUDPbitrate, bool bReverse) */
int  EndpointService::addIperfClient(QString cfg)
{
    qDebug() << "TODO:addIperfClient:" << cfg;
    //TODO: parser <pair></pair>
    IperfXmlPair *pair=new IperfXmlPair(cfg);

    //TODO
    /*
    qDebug() << "add iperf client connect to "<< ServerIP << " port:" << ServerPort;
    IperfClient *c=new IperfClient(ServerIP,ServerPort,
                                   iMode, iTime, iOmit,
                                   iBuffer, iWindow,
                                   iUDPbitrate, bReverse);
    connect(c, SIGNAL(finished()), this, SLOT(onIperfClientFinished()));
    connect(c, SIGNAL(sigStdout(QString)), this, SLOT(onIperfClientStdout(QString)));
    connect(c, SIGNAL(sigStderr(QString)), this, SLOT(onIperfClientStderr(QString)));

    QThread *t=new QThread;
    c->moveToThread(t);
    connect(t, SIGNAL(started()), c, SLOT(start()));
    connect(t, SIGNAL(finished()), this, SLOT(deleteLater()));

    iperfClients.append(c);
    iperfClientThreads.append(t);
    //TODO: error return not 0 value?
    */
    return 0;
}
int EndpointService::startAllIperfClient(int notUse){
    Q_UNUSED(notUse);
    int iMax = iperfClientThreads.count();
    if (iMax>0){
        for (int i=0; i< iperfClientThreads.count(); i++)
        {
            startIperfClient(i);
        }
    }
    return 0;
}
int EndpointService::stopAllIperfClient(int notUse){
    Q_UNUSED(notUse);
    int iMax = iperfClientThreads.count();
    if (iMax>0){
        for (int i=0; i< iperfClientThreads.count(); i++)
        {
            stopIperfClient(i);
        }
    }
    return 0;
}
bool EndpointService::startIperfClient(int idx)
{
    int iMax=iperfClientThreads.count();
    if (idx >= iMax){
        return false;
    }
    QThread  *t = iperfClientThreads.at(idx);
    t->start();
    return true;
}
bool EndpointService::stopIperfClient(int idx){
    int iMax=iperfClients.count();
    if (idx >= iMax){
        return false;
    }
    IperfClient  *c = iperfClients.at(idx);
    c->setStop();
    //TODO: wait thread stop?
    iperfClients.removeAt(idx);
    iperfClientThreads.removeAt(idx);
    return true;
}
bool EndpointService::isIperfClientRunning(int idx){
    int iMax=iperfClients.count();
    if (idx >= iMax){
        return false;
    }
    IperfClient  *c = iperfClients.at(idx);
    return c->isRunning();
}
void EndpointService::onIperfClientFinished()
{
    qDebug() << "onIperfClientFinished";
}
void EndpointService::onIperfClientStdout(QString line)
{
    qDebug() << "IperfClient: " << line;
}
void EndpointService::onIperfClientStderr(QString line)
{
    qDebug() << "IperfClient err: " << line;
}
