#include "iperfclient.h"
#include <QCoreApplication>
#include "sleeper.h"

#include <QDebug>

IperfClient::IperfClient(QString ServerIP,int ServerPort,
                         int iMode, int iTime, int iOmit,
                         int iBuffer, int iWindow,
                         int iUDPbitrate, bool bReverse,
                         QObject *parent):
    IperfCore(ServerPort)
{
    Q_UNUSED(parent);

#if USE_QPROCRESS==1
    mStop=false;
    m_ServerIP = QString("-c %1").arg(ServerIP);
    m_args.insert(0 , m_ServerIP);

    switch (iMode) {
    case 1:
        m_Mode = QString("-u");
        break;
    case 2:
        m_Mode = QString("--sctp");
        break;
    case 0:
    default:
        m_Mode = QString("");
        break;
    }
    m_args.append(m_Mode);

    if (iTime>0){
        m_Time = QString("-t %1").arg(iTime);
    }else{
        //TODO: -n, --bytes
        // -k, --blockcount
    }
    m_args.append(m_Time);

    if (iOmit>0){
        m_Omit = QString("-O %1").arg(iOmit);
        m_args.append(m_Omit);
    }
    if (iBuffer>0) {
        //TODO: -l, --length , buffer length
    }
    if (iWindow>0) {
        //TODO: -w, --window    #[KMG]    set window size / socket buffer size
    }
    if (iUDPbitrate>=0){
        //TODO: -b, --bitrate #[KMG][/#]  target bitrate in bits/sec (0 for unlimited)
        m_UDPbitrate = QString("-b %1").arg(iUDPbitrate);
    }
    if (bReverse){
        m_args.append("-R");
    }

#endif
}
IperfClient::~IperfClient()
{

}

void IperfClient::start()
{
#if USE_QPROCRESS==1
    if (m_Iperf.length()>0){
        iperf = new QProcess();
        connect( iperf, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadIperfOut()) );
        connect( iperf, SIGNAL(readyReadStandardError()), this, SLOT(ReadIperfErr()) );

        QString cmd;
        cmd.append(m_Iperf).append(" ").append(m_args.join(" "));
        qDebug() << "cmd: " << cmd;
        iperf->start(cmd);
        iperf->waitForStarted();
        while ((iperf->state()== QProcess::Running) && (! mStop)){
            QCoreApplication::processEvents();
            Sleeper::sleep(1);
        }
        emit finished();
    }
#else
    //TODO: use iperf3 API to control client
#endif
}
