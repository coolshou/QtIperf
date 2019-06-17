#ifndef IPERFD_H
#define IPERFD_H

#include <QObject>

#include "logger.h"
//#include "endpoint.h"
#include "broadcast.h"
#include "endpointservice.h"
#include "json_rpc_tcp_server.h"

class IperfD : public QObject
{
    Q_OBJECT
public:
    explicit IperfD(QObject *parent = nullptr);
    void defaultConfig();
    //QtService
    void pause()
    {
        disabled = true;
    }

    void resume()
    {
        disabled = false;
    }

private slots:
    void on_clientConnected(QObject* client_socket);

signals:
    void finished();

public slots:
    bool start();

private:
    bool isHost64Bit();
    void Debug(QString mlog);

private:
    Logger *m_logger;
    QString sSystemName;
    bool bSystemX64; //
    jcon::JsonRpcServer* rpc_server;
    EndpointService *epService; //
    Broadcast *bc;
    //QtService
    bool disabled;
};

#endif // IPERFD_H
