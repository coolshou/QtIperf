#ifndef ENDPOINTCLIENT_H
#define ENDPOINTCLIENT_H

#include <QObject>
#include <json_rpc_tcp_client.h>
#include <QNetworkReply>
#include <QDomElement>
#include <QTimer>
#include <QEventLoop>

#include "../src/const.h"
#include "../src/iperfxmlcfg.h"

struct LastResult {
    int statue;
    QString result;
};

class EndpointClient : public QObject
{
    Q_OBJECT
public:
    explicit EndpointClient(QObject *parent ,
                            QString pairCfg,
                            QString rpcHost, int rpcPort=JCONRPC_PORT);
    LastResult getLastResult();
    void getIperfVersion();
    void addServer();
    void startServer();
    void stopServer();

    void addClient();
    void startClient();
    void stopClient();

signals:
    void sig_error(int error, const QString &message);

private slots:
    void on_getIperfVersion(const QVariant &);
    void testResponse(const QVariant &);
    void testFault(int, const QString &);
    void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void on_networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);
    //void onTimeout();
    void on_jsonrpc_Connected(QObject* socket);
    void on_jsonrpc_Disconnected(QObject* socket);
    void on_jsonrpc_socketError(QObject* socket, QAbstractSocket::SocketError error);

private:
    //MaiaXmlRpcClient *rpc;
    jcon::JsonRpcClient* rpc_client;
    QString m_RPCHost;
    int m_RPCPort;
    QString m_pairCfg;
    QTimer timer;
    QEventLoop loop;
    //
    LastResult m_LastResult; // hold last cmd's result
    void debug(QString msg);
};

#endif // ENDPOINTCLIENT_H
