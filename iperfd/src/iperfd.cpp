#include "iperfd.h"
#include "iperfserver.h"

#include <QSysInfo>
#include <QStandardPaths>
#include <QDir>
#include <QSettings>

#include <QDebug>


IperfD::IperfD(QObject *parent) : QObject(parent)
{
    QSysInfo sysinfo;
    //setting,
    //log file
    QString cfgpath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir cfg = QDir(QString("%1/IperfD.log").arg(cfgpath));
    //qDebug() << "cfg: " << cfg.absolutePath();
    m_logger =new Logger(this, cfg.absolutePath());
    //m_logger =new Logger(this, "test.log");
    Debug("##### System Information #####");
    sSystemName = sysinfo.prettyProductName();
    Debug("product name and version: " + sSystemName);
    bSystemX64 =isHost64Bit();
    //qDebug() << "x64: " << bSystemX64;
    Debug("##############################");
    rpc_server = new jcon::JsonRpcTcpServer(parent);
    connect(rpc_server,SIGNAL(clientConnected(QObject*)),this, SLOT(on_clientConnected(QObject*)));
    bc = new Broadcast();
}
void IperfD::Debug(QString mlog){
    m_logger->write(mlog);
}
bool IperfD::isHost64Bit() {
  static bool h = QSysInfo::currentCpuArchitecture().contains(QLatin1String("64"));
  return h;
}
void IperfD::on_clientConnected(QObject* client_socket){
    qDebug() << "on_clientConnected:"; // <<QTcpSocket(client_socket) ;

    //QTcpSocket(client_socket)
    //QTcpSocket *tcpsck = qobject_cast<QTcpSocket *>(client_socket);

    //Q_UNUSED(client_socket);
}
bool IperfD::start()
{
    m_logger->write("start");
    //xmlrpc server: wait for command
    //start rpc server
    Debug("start rpc server");
    epService = new EndpointService();
    rpc_server->registerServices({ epService });
    rpc_server->listen(JCONRPC_PORT);

    //ep = new EndpointService(this);
    Debug("rpc server created");
    bc->startBroadcasting();

    return true;
    /*
    if (0){
        //following test iperf server & client
        //iperfserver *s = new iperfserver(this, 5201);
        int port = ep->AddIperfServer();
        bool s = ep->startIperfServer();
        //int port=5201;
        if (s) {
            ep->AddIperfClient("127.0.0.1", port, 0);//TCP
            //ep->AddIperfClient("127.0.0.1", port, 1);//UDP
            ep->startIperfClient();//start all client
        }
        //signal finish
        //emit finished();
    }*/
}

