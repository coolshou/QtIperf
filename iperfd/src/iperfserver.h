#ifndef IPERFSERVER_H
#define IPERFSERVER_H

//#include <QObject>
#include <QMutex>
#include <QProcess>
#include "iperfcore.h"
#include "../src/const.h"
#include "iperf_config.h"
#include "iperf_api.h"

class IperfServer : public IperfCore
{
    Q_OBJECT
public:
    //explicit iperfserver(QObject *parent = nullptr);
    explicit IperfServer(QObject *parent, int port=BASE_PORT);
    virtual ~IperfServer();
    //virtual bool event(QEvent *event);
    //virtual bool eventFilter(QObject *watched, QEvent *event);
    //void setStop();

public slots:
    void start();

private:
    //QMutex mutex;
    //bool mStop; //stop running
};

#endif // IPERFSERVER_H
