#ifndef IPERFCORE_H
#define IPERFCORE_H

#include <QObject>
#include <QMutex>
#include <QProcess>
#include "../src/const.h"
#include "iperf_api.h"

#define USE_QPROCRESS 1 //use QProcress to run iperf
/*
enum iperfformat{
    k="Kbits",
    m="Mbits",
    g="Gbits",
    t="Tbits",
    K="KByte",
    M="MByte",
    G="GByte",
    T="TByte"
};*/

#define REPORT "Accepted connection from"
#define REPORT_START_TEST "Starting Test:"
#define REPORT_END_TEST "Test Complete. Summary Results:"
#define REPORT_HEAD "[ ID]"
#define REPORT_SUM "[SUM]"
#define REPORT_CPU_UTILIZATION "CPU Utilization:"

/*TCP
iperf stdout:  "Accepted connection from 127.0.0.1, port 47606"
iperf stdout:  "      Cookie: lkcxcfnmw3dsu7uldl4kakt3rnncasf5ifjr"
iperf stdout:  "      TCP MSS: 0 (default)"
iperf stdout:  "[  5] local 127.0.0.1 port 5201 connected to 127.0.0.1 port 47608"
iperf stdout:  "[  8] local 127.0.0.1 port 5201 connected to 127.0.0.1 port 47610"
iperf stdout:  "Starting Test: protocol: TCP, 2 streams, 131072 byte blocks, omitting 0 seconds, 10 second test, tos 0"
iperf stdout:  "[ ID] Interval           Transfer     Bitrate"
iperf stdout:  "[  5]   0.00-1.00   sec  3.75 GBytes  32215904 Kbits/sec                  "
iperf stdout:  "[  8]   0.00-1.00   sec  3.75 GBytes  32214755 Kbits/sec                  "
iperf stdout:  "[SUM]   0.00-1.00   sec  7.50 GBytes  64430759 Kbits/sec                  "
iperf stdout:  "- - - - - - - - - - - - - - - - - - - - - - - - -"
iperf stdout:  "Test Complete. Summary Results:"
iperf stdout:  "[ ID] Interval           Transfer     Bitrate"
iperf stdout:  "[  5] (sender statistics not available)"
iperf stdout:  "[  5]   0.00-10.04  sec  27.9 GBytes  23908150 Kbits/sec                  receiver"
iperf stdout:  "[  8] (sender statistics not available)"
iperf stdout:  "[  8]   0.00-10.04  sec  28.0 GBytes  23966657 Kbits/sec                  receiver"
iperf stdout:  "[SUM] (sender statistics not available)"
iperf stdout:  "[SUM]   0.00-10.04  sec  55.9 GBytes  47874807 Kbits/sec                  receiver"
iperf stdout:  "CPU Utilization: local/receiver 2.6% (0.1%u/2.5%s), remote/sender 2.7% (0.5%u/2.2%s)"
*/

/*UDP
iperf stdout:  "Accepted connection from 127.0.0.1, port 47570"
iperf stdout:  "      Cookie: mx3iy5f6g6ukhzpbp75m6fvfpadqwwshgi2p"
iperf stdout:  "[  5] local 127.0.0.1 port 5201 connected to 127.0.0.1 port 35322"
iperf stdout:  "[  6] local 127.0.0.1 port 5201 connected to 127.0.0.1 port 57379"
iperf stdout:  "Starting Test: protocol: UDP, 2 streams, 21888 byte blocks, omitting 0 seconds, 10 second test, tos 0"
iperf stdout:  "[ ID] Interval           Transfer     Bitrate         Jitter    Lost/Total Datagrams"
iperf stdout:  "[  5]   0.00-1.00   sec   128 KBytes  1050 Kbits/sec  0.006 ms  0/6 (0%)  "
iperf stdout:  "[  6]   0.00-1.00   sec   128 KBytes  1050 Kbits/sec  0.004 ms  0/6 (0%)  "
iperf stdout:  "[SUM]   0.00-1.00   sec   256 KBytes  2101 Kbits/sec  0.005 ms  0/12 (0%)  "
iperf stdout:  "- - - - - - - - - - - - - - - - - - - - - - - - -"
iperf stdout:  "Test Complete. Summary Results:"
iperf stdout:  "[ ID] Interval           Transfer     Bitrate         Jitter    Lost/Total Datagrams"
iperf stdout:  "[  5] (sender statistics not available)"
iperf stdout:  "[  5]   0.00-10.04  sec  1.25 MBytes  1047 Kbits/sec  0.005 ms  0/60 (0%)  receiver"
iperf stdout:  "[  6] (sender statistics not available)"
iperf stdout:  "[  6]   0.00-10.04  sec  1.25 MBytes  1047 Kbits/sec  0.003 ms  0/60 (0%)  receiver"
iperf stdout:  "[SUM]   0.00-10.04  sec  2.50 MBytes  2093 Kbits/sec  0.004 ms  0/120 (0%)  receiver"
iperf stdout:  "CPU Utilization: local/receiver 0.0% (0.0%u/0.0%s), remote/sender 2.7% (0.5%u/2.2%s)"
*/

/*SCTP (CPU usage high?)
iperf stdout:  "Accepted connection from 127.0.0.1, port 47616"
iperf stdout:  "      Cookie: fyckmzoadiuklof6loatbmzsfilc2lgwun7m"
iperf stdout:  "[  5] local 127.0.0.1 port 5201 connected to 127.0.0.1 port 33273"
iperf stdout:  "[  8] local 127.0.0.1 port 5201 connected to 127.0.0.1 port 57866"
iperf stdout:  "Starting Test: protocol: SCTP, 2 streams, 65536 byte blocks, omitting 0 seconds, 10 second test, tos 0"
iperf stdout:  "[ ID] Interval           Transfer     Bitrate"
iperf stdout:  "[  5]   0.00-1.00   sec   384 KBytes  3145 Kbits/sec                  "
iperf stdout:  "[  8]   0.00-1.00   sec   320 KBytes  2621 Kbits/sec                  "
iperf stdout:  "[SUM]   0.00-1.00   sec   704 KBytes  5766 Kbits/sec                  "
iperf stdout:  "- - - - - - - - - - - - - - - - - - - - - - - - -"
iperf stdout:  "Test Complete. Summary Results:"
iperf stdout:  "[ ID] Interval           Transfer     Bitrate"
iperf stdout:  "[  5] (sender statistics not available)"
iperf stdout:  "[  5]   0.00-10.03  sec  4.75 MBytes  3971 Kbits/sec                  receiver"
iperf stdout:  "[  8] (sender statistics not available)"
iperf stdout:  "[  8]   0.00-10.03  sec  84.9 MBytes  71011 Kbits/sec                  receiver"
iperf stdout:  "[SUM] (sender statistics not available)"
iperf stdout:  "[SUM]   0.00-10.03  sec  89.7 MBytes  74982 Kbits/sec                  receiver"
iperf stdout:  "CPU Utilization: local/receiver 0.0% (0.0%u/0.0%s), remote/sender 87.7% (1.4%u/86.3%s)"
*/
class IperfCore : public QObject
{
    Q_OBJECT
public:
    explicit IperfCore(int iPort=BASE_PORT, QString sFormat="k",
                       QString sBind="",int iVerbose=1, bool bJson=false,
                       QString log="", bool bForceflush=true,
                       bool bDebug=false,
                       QObject *parent = nullptr);
    /*
      -p, --port      #         server port to listen on/connect to
      -f, --format   [kmgtKMGT] format to report: Kbits, Mbits, Gbits, Tbits
      -F, --file name           xmit/recv the specified file
      -A, --affinity n/n,m      set CPU affinity
      -B, --bind      <host>    bind to the interface associated with the address <host>
      -V, --verbose             more detailed output
      -J, --json                output in JSON format
      --logfile f               send output to a log file
      --forceflush              force flushing output at every interval
      -d, --debug               emit debugging output
      -v, --version             show version information and quit
      -h, --help                show this message and quit

    */
    virtual ~IperfCore();
    QString version();
    void setStop();
    bool isRunning();

signals:
    void finished();
    void sigStdout(QString line);
    void sigStderr(QString line);

//public slots:
//    void start();
#if USE_QPROCRESS==1
public slots:
    void ReadIperfOut();
    void ReadIperfErr();

private slots:
    void onGetVersion();
#endif

protected:
#if USE_QPROCRESS==0
    struct iperf_test *test;
#else
    QStringList m_args;
    QString m_Iperf; //iperf exe file full path
    QProcess *iperf=nullptr;
    QMutex mutex;
    bool mStop; //stop running
    bool mRunning;
#endif

private:
#if USE_QPROCRESS==1
    QProcess *info;
    QString m_iperfVersion;
    //args for iperf3 server/client
    QString m_Port;
    QString m_Format;
    QString m_Interval;
    QString m_Bind;
    QString m_Verbose;
    QString m_Json;
    QString m_log;
    QString m_Forceflush;
    QString m_Debug;

    //QProcess *info;
#endif

};

#endif // IPERFCORE_H
