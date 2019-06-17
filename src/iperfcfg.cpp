#include "iperfcfg.h"

iperfcfg::iperfcfg(QObject *parent) : QObject(parent)
{
    jDoc= QJsonDocument();
    jCommObj = QJsonObject();
    jClientObj = QJsonObject();
    defaultClientConfig();
    jCommObj.insert("Client", jClientObj);
    defaultConfig();
    jDoc.setObject(jCommObj);

    qDebug() << "JSON: " << jDoc.toJson(QJsonDocument::Indented);
}
void iperfcfg::defaultConfig() {
    //create a json format document for iperf server/client
    /*  { "-p":5201,
          "-f":"k", //[kmgKMG]
          "-V":1,
          "-B":"",
          "-J":false,
          "--logfile":"",
          "--forceflush":true, //force flush stdout
          "-d":false, //debug
        }
     */
    qDebug() << "defaultConfig";
    jCommObj.insert("-p", 5201);
    jCommObj.insert("-f", QString("k"));
    jCommObj.insert("-V", 1);
    jCommObj.insert("-B", QString(""));
    jCommObj.insert("-J", false);
    jCommObj.insert("--logfile", QString(""));
    jCommObj.insert("--forceflush" ,true);
    jCommObj.insert("-d", false);

}
void iperfcfg::defaultClientConfig(){
    /* client
        "client" {
            "-c":"127.0.0.1",
            "testMode":"" // "TCP":"", "UDP":"-u", "SCTP":"--sctp"
            "testMethod" {
                "-t":10, // time in seconds to transmit for (default 10 secs)
                "-n":0,   // [KMG] number of bytes to transmit (instead of -t)
                "-k":0,   // [KMG] number of blocks (packets) to transmit (instead of -t or -n)
            }
            "-O":0,
            "-l":0, //0: TCP: 128K, UDP: 1460 or dynamic
            "-w":0, //0: not set windows size use default
            "-R":false,
            "--bitrate":0, //target bitrate in bits/sec (0 for unlimited)
                            (default 1 Mbit/sec for UDP, unlimited for TCP)
        }
    */
    qDebug() << "defaultClientConfig";
    jClientObj.insert("-c", QString("127.0.0.1"));
    jClientObj.insert("testMode", QString("")); // "TCP":"", "UDP":"-u", "SCTP":"--sctp"
    jClientObj.insert("-t", 10); //testMethod: -t, TODO: -n,-k
    jClientObj.insert("-O", 0); //omit ? sec, 0: no omit
    jClientObj.insert("-l", 0); //buffer length, 0: not set use default
    jClientObj.insert("-w", 0); //windows size, 0: not set use default
    jClientObj.insert("-R", false);
    jClientObj.insert("--bitrate", 0);
}
int iperfcfg::set_ServerIP(QString ip)
{
    QJsonValue value = jClientObj.value("-c");
    qDebug() << "org ip:" << value;
    //TODO: why can not update value????bug???
    jClientObj.remove("-c");
    jClientObj.insert("-c",ip);
    value = jClientObj.value("-c");
    qDebug() << "new ip:" << value;
    return 0;
}
int iperfcfg::set_ServerPort(int port)
{
    jCommObj.value("-p")=port;
    return 0;
}
int iperfcfg::set_TestMode(testModeType tMode){
    switch (tMode) {
       case testModeType::UDP:
            jCommObj.value("testMode")="-u";
            break;
       case testModeType::SCTP:
            jCommObj.value("testMode")="--sctp";
            break;
       case testModeType::TCP:
            jCommObj.value("testMode")="";
            break;
    }

    return 0;
}

QString iperfcfg::get_cfgJSON(){
    //return QString of json
    return QString::fromLocal8Bit(jDoc.toJson());
}
