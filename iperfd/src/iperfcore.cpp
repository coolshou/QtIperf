#include "iperfcore.h"

//iperf3 static link
//./configure "LDFLAGS=--static" CFLAGS=-static CPPFLAGS=-static --enable-static --disable-shared
//cross-compile i686
/*
./configure --host=i686-linux-gnu \
 "LDFLAGS=--static" CFLAGS=-static CPPFLAGS=-static --enable-static --disable-shared
#windows
https://github.com/hejin/iperf3-for-windows-static

./configure --host=mingw32  \
CC=/opt/mxe-qt5.11/usr/bin/i686-w64-mingw32.static-gcc  \
CPP=/opt/mxe-qt5.11/usr/bin/i686-w64-mingw32.static-cpp \
CXX=/opt/mxe-qt5.11/usr/bin/i686-w64-mingw32.static-g++  \
AR=/opt/mxe-qt5.11/usr/bin/i686-w64-mingw32.static-ar \
LD=/opt/mxe-qt5.11/usr/bin/i686-w64-mingw32.static-ld \
 "LDFLAGS=--static" CFLAGS=-static CPPFLAGS=-static --enable-static --disable-shared
 */

#include <QDir>
#include <QCoreApplication>
#include <QSysInfo>
#include <QFileInfo>

#include <QDebug>

IperfCore::IperfCore(int iPort, QString sFormat,
                     QString sBind, int iVerbose, bool bJson,
                     QString log, bool bForceflush,
                     bool bDebug,
                     QObject *parent) :
    QObject(parent)
{

#if USE_QPROCRESS==1
    //following are QProcress base
    m_Port = QString("-p %1").arg(iPort);
    m_Format = QString("-f%1").arg(sFormat);
    m_Verbose = QString("-V %1").arg(iVerbose);

    m_args.append(m_Port);
    m_args.append(m_Format);
    m_args.append(m_Verbose);

    if (sBind != ""){
        m_Bind = QString("-B %1").arg(sBind);
        m_args.append(m_Bind);
    }
    if (bJson){
        m_Json= QString("-J");
        m_args.append(m_Json);
    }
    if (log!=""){
        //this will cause all stdout to a log file!!
        m_log=QString("--logfile %1").arg(log);
        m_args.append(m_log);
    }
    if (bForceflush) {
        m_Forceflush=QString("--forceflush");
        m_args.append(m_Forceflush);
    }
    if (bDebug){
        m_Debug=QString("-d");
        m_args.append(m_Debug);
    }
    //current path
    //qDebug() << "appPath:" <<  qApp->property("APP_PATH").toString();
    QString app_path=qApp->property("APP_PATH").toString();
    //iperf path
    QSysInfo sysinfo;
#if defined(Q_OS_LINUX)
    QString iperf_exec="iperf3";
    m_Iperf=QDir(app_path + "/bin/"+
                  sysinfo.kernelType()+"/"+
                  sysinfo.currentCpuArchitecture()+"/").filePath(iperf_exec);
#elif defined(Q_OS_WIN32)
    QString iperf_exec="iperf3.exe";
    m_Iperf=QDir(app_path + "/bin/"+
                  sysinfo.kernelType()+"/").filePath(iperf_exec);
#else
    qDebug() << "Not supported platform for iperf3";
    m_Iperf="";
    //return -1;
#endif
    bool fileExists = QFileInfo::exists(m_Iperf) && QFileInfo(m_Iperf).isFile();

    //if (QDir(m_Iperf).exists()){
    if (fileExists){
        //version info
        //QString s= this->version();
        //qDebug()<< "iperf ver:" <<s;
    } else {
        qDebug() << "file:" << m_Iperf << " not found!!";
        //return -1;
    }

    //server/client args
#else
    test = iperf_new_test();
    if ( test == NULL ) {
        //fprintf( stderr, "%s: failed to create test\n", argv0 );
        qDebug() << "failed to create test";
        exit( EXIT_FAILURE );
    }
    iperf_defaults( test );
    iperf_set_verbose( test, iVerbose );
    iperf_set_test_reporter_interval( test, iInterval);
    //iperf_set_test_stats_interval

    if (bJson){
        //json output will output on test finish, not realtime output!!
        iperf_set_test_json_output( test, 1);
    }
    iperf_set_test_server_port( test, iPort );

    Q_UNUSED(sFormat);
    Q_UNUSED(sBind);
    Q_UNUSED(log);
    Q_UNUSED(bForceflush);
    Q_UNUSED(bDebug);
#endif

}
IperfCore::~IperfCore()
{
#if USE_QPROCRESS==1
    if (iperf){
        if (iperf->state() != QProcess::NotRunning ) {
            iperf->terminate();
        }
    }
#else
    iperf_free_test( test );
#endif
}
void IperfCore::setStop()
{
    mutex.lock();
    mStop=true;
    mutex.unlock();
}
//require iperf_api add iperf_get_version() fuction
QString IperfCore::version()
{
#if USE_QPROCRESS==1
    QString ver="";
    QStringList args;
    //QProcess *info= new QProcess();
    info= new QProcess();
    args.append("--version");
    info->start(m_Iperf, args);
    //TODO: this  will block at most 2 sec to wait procress finish
    while (not info->waitForFinished(1000)){
        QCoreApplication::processEvents();

    }
    QByteArray r;
    r=info->readAllStandardOutput();
    ver = QString::fromLocal8Bit(r.data());

    QStringList ls;
    QString l;
    ls=ver.split("\n");
    if (ls.length()>0){
        l = ls[0];
    }
    ls = l.split(" ");
    if (ls.length()>0){
        ver = ls[1];
    }
    /*
    // catch data output
    connect(info, SIGNAL(readyRead()), this, SLOT(onGetVersion()));

    // delete process instance when done, and get the exit status to handle errors.
    connect(info, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [=](int exitCode, QProcess::ExitStatus {
        qDebug()<< "process exited with code " << exitCode;
        info->deleteLater();
    });
    */
    return ver;
#else
    char *v;
    v= iperf_get_iperf_version(); //iperf 3.6
    return QString(v);
#endif
}

#if USE_QPROCRESS==1
void IperfCore::ReadIperfOut()
{
    mRunning=true;
    QByteArray baData;
    QString data;
    QStringList lines;
    QProcess *p = dynamic_cast<QProcess *>( sender() );
    if (p){
      baData = p->readAllStandardOutput();
      if (baData.length()>0){
        data = QString::fromLocal8Bit(baData.data());
        lines = data.split("\n");
        foreach (QString line, lines) {
            //parser output line by line
            if (line.length()>0){
                //qDebug() << "iperf stdout: " << line;
                emit sigStdout(line);
            }
        }
      }
    }
}
void IperfCore::ReadIperfErr()
{
    QByteArray baData;
    QString data;
    QStringList lines;

    QProcess *p = dynamic_cast<QProcess *>( sender() );
    if (p){
      baData = p->readAllStandardError();
      //parser error output
      if (baData.length()>0){
          data = QString::fromLocal8Bit(baData.data());
          lines = data.split("\n");
          foreach (QString line, lines) {
              if (line.length()>0){
                //qDebug() << "iperf stderr: " << line;
                emit sigStderr(line);
              }
          }
      }
    }
    mRunning=false;

}
void IperfCore::onGetVersion(){
    QByteArray a = info->readAll();
    QString d = QString::fromLocal8Bit(a.data());
    QStringList lines = d.split("\n" );
    m_iperfVersion = lines.value(0);
    qDebug() <<" version:" << m_iperfVersion;
}
#endif

bool IperfCore::isRunning(){
    return mRunning;
}

/*
bool iperfCore::event(QEvent *event){

}
bool iperfCore::eventFilter(QObject *watched, QEvent *event){

}
*/
/*
int iperfCore::getBasePort()
{
    return 5201;
}
*/
