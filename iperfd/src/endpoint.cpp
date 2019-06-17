#include "endpoint.h"
#include "iperfcore.h"
#include "iperfserver.h"
#include "../src/const.h"

#include "../src/iperfxmlcfg.h"

#include <QDebug>

Endpoint::Endpoint(QObject *parent) :
    QObject(parent)
{
    server = new MaiaXmlRpcServer(MAIAXMLRPC_PORT, this);
    //common control cmd
    server->addMethod("iperf.version", this, "getIperfVersion");
    //iperf server cmd
    server->addMethod("iperf.server.add", this, "addIperfServer");
    server->addMethod("iperf.server.index", this, "getIperfServerIdx");
    server->addMethod("iperf.server.start", this, "startIperfServer");
    server->addMethod("iperf.server.stop", this, "stopIperfServer");
    server->addMethod("iperf.server.startAll", this, "startAllIperfServer");
    server->addMethod("iperf.server.stopAll", this, "stopAllIperfServer");
    server->addMethod("iperf.server.isRunning", this, "isIperfServerRunning");
    //iperf client cmd
    server->addMethod("iperf.client.add", this, "addIperfClient");
    server->addMethod("iperf.client.index", this, "getIperfClientIdx");
    server->addMethod("iperf.client.start", this, "startIperfClient");
    server->addMethod("iperf.client.stop", this, "stopIperfClient");
    server->addMethod("iperf.client.startAll", this, "startAllIperfClient");
    server->addMethod("iperf.client.stopAll", this, "stopAllIperfClient");
    server->addMethod("iperf.client.isRunning", this, "isIperfClientRunning");
    //qDebug() << "endpoint created";

    bc = new Broadcast();
    bc->startBroadcasting();
}

QString Endpoint::getIperfVersion(){
    IperfCore iperf;
    QString s = iperf.version();
    //qDebug() << "getIperfVersion: " << s;
    return s;
}

//iperf server ####################################################
int Endpoint::addIperfServer(QString cfg) {
    Q_UNUSED(cfg);
    //qDebug() << "AddIperfServer cfg:" << cfg;
    int idx = this->getIperfServerIdx();

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
int Endpoint::getIperfServerIdx(){
    //return iperfServers.size();
    return iperfServerThreads.size();
}
void Endpoint::startAllIperfServer(){
    int iMax = iperfServerThreads.count();
    if (iMax>0){
        for (int i=0; i<iMax ; i++)
        {
            startIperfServer(i);
        }
    }

}
void Endpoint::stopAllIperfServer(){
    int iMax = iperfServerThreads.count();
    if (iMax>0){
        for (int i=0; i< iperfServerThreads.count(); i++)
        {
            stopIperfServer(i);
        }
    }
}
bool Endpoint::startIperfServer(int idx)
{
    int iMax=iperfServerThreads.count();
    if (idx >= iMax){
        return false;
    }
    QThread  *t = iperfServerThreads.at(idx);
    t->start();
    return true;
}
bool Endpoint::stopIperfServer(int idx){
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
bool Endpoint::isIperfServerRunning(int idx){
    int iMax=iperfServers.count();
    if (idx >= iMax){
        return false;
    }
    IperfServer  *c = iperfServers.at(idx);
    return c->isRunning();
}
void Endpoint::onIperfServerFinished()
{
    qDebug() << "onIperfServerFinished";
}
void Endpoint::onIperfServerStdout(QString line)
{
    qDebug() << "IperfServer: " << line;
}
void Endpoint::onIperfServerStderr(QString line)
{
    qDebug() << "IperfServer err: " << line;
}

//iperf client ####################################################
/*int  Endpoint::addIperfClient(QString ServerIP,int ServerPort,
                              int iMode, int iTime, int iOmit,
                              int iBuffer, int iWindow,
                              int iUDPbitrate, bool bReverse) */
int  Endpoint::addIperfClient(QString cfg)
{
    //TODO: parser <pair></pair>
    IperfXmlPair *pair=new IperfXmlPair(cfg);
    /*
    QDomDocument doc;
    doc.setContent(cfg);
    qDebug() << "addIperfClient" << doc.toString();
    QDomNodeList portLs= doc.elementsByTagName("port");
    int ServerPort = portLs.at(0).toElement().attribute("value").toInt();
    QDomNodeList clientLs= doc.elementsByTagName("client");
    QDomElement clientElm = clientLs.at(0).toElement();
    QDomNodeList serverLs = clientElm.elementsByTagName("ServerIP");
    QString ServerIP = serverLs.at(0).toElement().attribute("vlaue");
    */
    //QDomNodeList clientLs

    //TODO
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
void Endpoint::startAllIperfClient(){
    int iMax = iperfClientThreads.count();
    if (iMax>0){
        for (int i=0; i< iperfClientThreads.count(); i++)
        {
            startIperfClient(i);
        }
    }
}
void Endpoint::stopAllIperfClient(){
    int iMax = iperfClientThreads.count();
    if (iMax>0){
        for (int i=0; i< iperfClientThreads.count(); i++)
        {
            stopIperfClient(i);
        }
    }
}
bool Endpoint::startIperfClient(int idx)
{
    int iMax=iperfClientThreads.count();
    if (idx >= iMax){
        return false;
    }
    QThread  *t = iperfClientThreads.at(idx);
    t->start();
    return true;
}
bool Endpoint::stopIperfClient(int idx){
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
bool Endpoint::isIperfClientRunning(int idx){
    int iMax=iperfClients.count();
    if (idx >= iMax){
        return false;
    }
    IperfClient  *c = iperfClients.at(idx);
    return c->isRunning();
}
void Endpoint::onIperfClientFinished()
{
    qDebug() << "onIperfClientFinished";
}
void Endpoint::onIperfClientStdout(QString line)
{
    qDebug() << "IperfClient: " << line;
}
void Endpoint::onIperfClientStderr(QString line)
{
    qDebug() << "IperfClient err: " << line;
}
