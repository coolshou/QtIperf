#include "endpointclient.h"

#include <QDebug>

EndpointClient::EndpointClient(QObject *parent,
                               QString pairCfg,
                               QString rpcHost, int rpcPort) :
    QObject(parent)
{
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    m_pairCfg = pairCfg;
    m_RPCHost = rpcHost;
    m_RPCPort = rpcPort;
    //qDebug() << "create JsonRpcTcpClient";
    debug("create JsonRpcTcpClient");
    rpc_client =  new jcon::JsonRpcTcpClient(parent);
    connect(rpc_client, SIGNAL(socketConnected(QObject*)), this, SLOT(on_jsonrpc_Connected(QObject*)));
    connect(rpc_client, SIGNAL(socketDisconnected(QObject*)), this, SLOT(on_jsonrpc_Disconnected(QObject*)));
    connect(rpc_client, SIGNAL(socketError(QObject*,QAbstractSocket::SocketError)), this, SLOT(on_jsonrpc_socketError(QObject*,QAbstractSocket::SocketError)));

    rpc_client->connectToServer(m_RPCHost, m_RPCPort);
    /*
    rpc = new MaiaXmlRpcClient(QUrl(QString("http://%1:%2/RPC2").arg(m_RPCHost).arg(m_RPCPort)), this);

    QSslConfiguration config = rpc->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    rpc->setSslConfiguration(config);

    connect(rpc, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));
    connect(rpc, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            this, SLOT(on_networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
    */
    //doClient();
    //getIperfVersion();

}

void EndpointClient::getIperfVersion(){
    qsrand(std::time(nullptr));

    timer.start(3000);
    /*
    auto req = rpc_client->callAsync("getIperfVersion", 0);
    req->connect(req.get(),&jcon::JsonRpcRequest::result,
                [this](const QVariant& result) {
                    //qDebug() << "result of asynchronous RPC call:" << result;
                    on_getIperfVersion(result);
                });
    req->connect(req.get(), &jcon::JsonRpcRequest::error,
                 [this](int code, const QString& message, const QVariant& data) {
                     //qDebug() << "RPC error:" << message << " (" << code << ")";
                     testFault(code, message);
                     Q_UNUSED(data);
                 });
    loop.exec();
    */
    //sync call
    timer.start(3000);
    auto result = rpc_client->call("getIperfVersion", 0);
    loop.exec();

    if (result->isSuccess()) {
        on_getIperfVersion(result->result());
        //qDebug() << "result of synchronous RPC call:" << result->result();
        debug(QString("result of synchronous RPC call: %1").arg(result->toString()));
    } else {
        debug(QString("RPC error: %1").arg(result->toString()));
        //qDebug() << "RPC error:" << result->toString();
    }
}

void EndpointClient::addServer(){
    qsrand(std::time(nullptr));

    //QVariantList args;
    //args << QVariant(m_pairCfg);
    //QVariant q = QVariant(m_pairCfg);
    timer.start(3000);
    auto req = rpc_client->callAsync("addIperfServer", m_pairCfg);
    req->connect(req.get(),&jcon::JsonRpcRequest::result,
                [this](const QVariant& result) {
                    //qDebug() << "result of asynchronous RPC call:" << result;
                    testResponse(result);
                });
    req->connect(req.get(), &jcon::JsonRpcRequest::error,
                 [this](int code, const QString& message, const QVariant& data) {
                     //qDebug() << "RPC error:" << message << " (" << code << ")";
                     testFault(code, message);
                     Q_UNUSED(data);
                 });
    loop.exec();


    /*
    QVariantList args;
    timer.start(3000);
    args << QVariant(m_pairCfg);
    rpc->call("iperf.server.add", args,
              this, SLOT(testResponse(QVariant &)),
              this, SLOT(testFault(int, const QString &)));
    loop.exec();
    */
}
void EndpointClient::startServer(){
    //start iperf server
    qsrand(std::time(nullptr));

    timer.start(3000);
    auto req = rpc_client->callAsync("startAllIperfServer", 0);
    req->connect(req.get(),&jcon::JsonRpcRequest::result,
                [this](const QVariant& result) {
                    //qDebug() << "result of asynchronous RPC call:" << result;
                    testResponse(result);
                });
    req->connect(req.get(), &jcon::JsonRpcRequest::error,
                 [this](int code, const QString& message, const QVariant& data) {
                     //qDebug() << "RPC error:" << message << " (" << code << ")";
                     testFault(code, message);
                     Q_UNUSED(data);
                 });
    loop.exec();

    /*
    QVariantList args;
    timer.start(3000);
    rpc->call("iperf.server.startAll", args,
              this, SLOT(testResponse(QVariant &)),
              this, SLOT(testFault(int, const QString &)));
    loop.exec();
    */
}
void EndpointClient::stopServer(){
    qsrand(std::time(nullptr));

    timer.start(3000);
    auto req = rpc_client->callAsync("stopAllIperfServer", 0);
    req->connect(req.get(),&jcon::JsonRpcRequest::result,
                [this](const QVariant& result) {
                    //qDebug() << "result of asynchronous RPC call:" << result;
                    testResponse(result);
                });
    req->connect(req.get(), &jcon::JsonRpcRequest::error,
                 [this](int code, const QString& message, const QVariant& data) {
                     //qDebug() << "RPC error:" << message << " (" << code << ")";
                     testFault(code, message);
                     Q_UNUSED(data);
                 });
    loop.exec();

    /*
    QVariantList args;
    timer.start(3000);
    rpc->call("iperf.server.stopAll", args,
              this, SLOT(testResponse(QVariant &)),
              this, SLOT(testFault(int, const QString &)));
    loop.exec();*/
}
void EndpointClient::addClient(){
    qsrand(std::time(nullptr));

//    QVariantList args;
//    args << QVariant(m_pairCfg);

    timer.start(3000);
    auto req = rpc_client->callAsync("addIperfClient", m_pairCfg);
    req->connect(req.get(),&jcon::JsonRpcRequest::result,
                [this](const QVariant& result) {
                    //qDebug() << "result of asynchronous RPC call:" << result;
                    testResponse(result);
                });
    req->connect(req.get(), &jcon::JsonRpcRequest::error,
                 [this](int code, const QString& message, const QVariant& data) {
                     //qDebug() << "RPC error:" << message << " (" << code << ")";
                     testFault(code, message);
                     Q_UNUSED(data);
                 });
    loop.exec();

    /*
    QVariantList args;
    timer.start(3000);
    qDebug() << "addClient pair: " << m_pairCfg;
    //m_pairCfg.toDocument().toText();
    args << QVariant(m_pairCfg);
    rpc->call("iperf.client.add", args,
              this, SLOT(testResponse(QVariant &)),
              this, SLOT(testFault(int, const QString &)));
    loop.exec();*/
}
void EndpointClient::startClient(){
    //start iperf client
    qsrand(std::time(nullptr));

    timer.start(3000);
    auto req = rpc_client->callAsync("startAllIperfClient", 0);
    req->connect(req.get(),&jcon::JsonRpcRequest::result,
                [this](const QVariant& result) {
                    //qDebug() << "result of asynchronous RPC call:" << result;
                    testResponse(result);
                });
    req->connect(req.get(), &jcon::JsonRpcRequest::error,
                 [this](int code, const QString& message, const QVariant& data) {
                     //qDebug() << "RPC error:" << message << " (" << code << ")";
                     testFault(code, message);
                     Q_UNUSED(data);
                 });
    loop.exec();

  /*
    QVariantList args;
    timer.start(3000);
    rpc->call("iperf.client.startAll", args,
              this, SLOT(testResponse(QVariant &)),
              this, SLOT(testFault(int, const QString &)));
    loop.exec();*/
}
void EndpointClient::stopClient(){
    qsrand(std::time(nullptr));

    timer.start(3000);
    auto req = rpc_client->callAsync("stopAllIperfClient", 0);
    req->connect(req.get(),&jcon::JsonRpcRequest::result,
                [this](const QVariant& result) {
                    //qDebug() << "result of asynchronous RPC call:" << result;
                    testResponse(result);
                });
    req->connect(req.get(), &jcon::JsonRpcRequest::error,
                 [this](int code, const QString& message, const QVariant& data) {
                     //qDebug() << "RPC error:" << message << " (" << code << ")";
                     testFault(code, message);
                     Q_UNUSED(data);
                 });
    loop.exec();

    /*
    QVariantList args;
    timer.start(3000);
    rpc->call("iperf.client.stopAll", args,
              this, SLOT(testResponse(QVariant &)),
              this, SLOT(testFault(int, const QString &)));
    loop.exec();*/
}
void EndpointClient::on_getIperfVersion(const QVariant &arg){
    m_LastResult.statue=0;
    m_LastResult.result=arg.toString();
    loop.quit();
}

void EndpointClient::testResponse(const QVariant &arg) {
    m_LastResult.statue=0;
    m_LastResult.result=arg.toString();
    //qDebug() << "testResponse: " << arg.toString();
    debug(QString("testResponse: %1").arg(arg.toString()));
    loop.quit();
}

void EndpointClient::testFault(int error, const QString &message) {
    m_LastResult.statue=error;
    m_LastResult.result=message;
    loop.quit();
    //qDebug() << "EEE:" << error << "-" << message;
    debug(QString("EEE: %1 - %2").arg(error).arg(message));
    emit sig_error(error, message);
}

void EndpointClient::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors) {
    m_LastResult.statue=reply->error();
    m_LastResult.result=reply->errorString();
    //qDebug() << "SSL Error:" << errors;
    debug(QString("SSL Error: %1").arg(m_LastResult.result));
    //reply->ignoreSslErrors(); // don't do this in real code! Fix your certs!
}
void EndpointClient::on_networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible){
    //qDebug() << "EndpointClient::on_networkAccessibleChanged";
    debug("EndpointClient::on_networkAccessibleChanged");
    Q_UNUSED(accessible);
}
LastResult EndpointClient::getLastResult()
{
    return m_LastResult;
}

void EndpointClient::on_jsonrpc_Connected(QObject* socket){
    //qDebug() << "EndpointClient::on_jconrpc_Connected";
    debug("EndpointClient::on_jconrpc_Connected");
    Q_UNUSED(socket);
}
void EndpointClient::on_jsonrpc_Disconnected(QObject* socket){
    //qDebug() << "EndpointClient::on_jsonrpc_Disconnected";
    debug("EndpointClient::on_jsonrpc_Disconnected");
    Q_UNUSED(socket);
}

void EndpointClient::on_jsonrpc_socketError(QObject* socket, QAbstractSocket::SocketError error){
    //qDebug() << "EndpointClient::on_jsonrpc_socketError: "<< error;
    debug(QString("EndpointClient::on_jsonrpc_socketError: %1").arg(error));
    Q_UNUSED(socket);
}
void EndpointClient::debug(QString msg){
    QDateTime now = QDateTime::currentDateTime();
    QString strBuffer = now.toString("yyyy-MM-dd hh:mm:ss");

    qDebug() << "[" << strBuffer << "][EndpointClient]" << msg;
}
