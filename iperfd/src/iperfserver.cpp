#include "iperfserver.h"
#include <QCoreApplication>
#include "sleeper.h"

#include <QDebug>

/*
Server specific:
  -s, --server              run in server mode
  -D, --daemon              run the server as a daemon
  -I, --pidfile file        write PID file
  -1, --one-off             handle one client connection then exit

 */
IperfServer::IperfServer(QObject *parent, int port):
    IperfCore(port)
{
    Q_UNUSED(parent);
    //this->iperfserver(parent);

#if USE_QPROCRESS==0
    iperf_set_test_role( test, 's' );
#else
    //mutex = QMutex();
    mStop=false;
    m_args.insert(0 ,"-s");

#endif
    //qDebug() << "iperfserver construct:" << port;

}
IperfServer::~IperfServer()
{

}

void IperfServer::start()
{
#if USE_QPROCRESS==0
    qDebug() << "iperfserver::start";
    int consecutive_errors = 0;
    for(;;){
        if ( iperf_run_server( test ) < 0 ) {
                //fprintf( stderr, "%s: error - %s\n\n", argv0, iperf_strerror( i_errno ) );
                qDebug() << "error - "<< iperf_strerror( i_errno ) <<"\n\n";
                ++consecutive_errors;
                if (consecutive_errors >= 5) {
                    //fprintf(stderr, "%s: too many errors, exiting\n", argv0);
                    qDebug() << "Too many errors, exiting\n";
                break;
                }
        } else {
            qDebug() << "no error";
            consecutive_errors = 0;
        }
        iperf_reset_test( test );
    }
    emit finished();
#else
    if (m_Iperf.length()>0){
        iperf = new QProcess();
        connect( iperf, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadIperfOut()) );
        connect( iperf, SIGNAL(readyReadStandardError()), this, SLOT(ReadIperfErr()) );

        QString cmd;
        cmd.append(m_Iperf).append(" ").append(m_args.join(" "));
        qDebug() << "cmd: " << cmd;
        iperf->start(cmd);
        //iperf->start(m_Iperf, m_args);
        iperf->waitForStarted();
        while ((iperf->state()== QProcess::Running) && (! mStop)){
            QCoreApplication::processEvents();
            Sleeper::sleep(1);
        }
        emit finished();
    }
#endif
}


/*
bool iperfserver::event(QEvent *event){

}
bool iperfserver::eventFilter(QObject *watched, QEvent *event){

}
*/
