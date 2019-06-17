#include "iperfxmlpair.h"

#include <QDebug>

IperfXmlPair::IperfXmlPair(QString xmlCfg, QObject *parent) :
    QObject(parent)
{
    QString cfg;
    if (xmlCfg.isEmpty()){
        //default xml config of pair
        cfg=DEFAULT_PAIR_CFG;
    } else {
        cfg=xmlCfg;
    }
    QDomDocument doc;
    doc.setContent(cfg);
    qDebug() << "addIperfClient" << doc.toString();
    QDomNodeList portLs= doc.elementsByTagName("port");
    mServerPort = portLs.at(0).toElement().attribute("value").toInt();
    /*
     * <format name=\"-f\" value=\"k\"/> \
    <verbose name=\"-V\" value=\"1\"/> \
    <json name=\"-J\" value=\"0\"/> \
    <forceflush name=\"--forceflush\" value=\"1\"/> \
    <debug name=\"-d\" value=\"0\"/> \
    <bind name=\"-B\" value=\"\"/> \
    */
    QDomNodeList clientLs= doc.elementsByTagName("client");
    //client
    QDomElement clientElm = clientLs.at(0).toElement();
    //
    QDomNodeList serverLs = clientElm.elementsByTagName("ServerIP");
    mServerIP = serverLs.at(0).toElement().attribute("value");
    //test mode: TCP, UDP
    QDomNodeList testModeLs = clientElm.elementsByTagName("testMode");
    QString testMode = testModeLs.at(0).toElement().attribute("name");
    if (testMode.compare("TCP")){
        iTestMode = testModeType::TCP;
    } else if (testMode.compare("UDP")){
        iTestMode = testModeType::UDP;
    } else {
        iTestMode = testModeType::SCTP;
    }
    //test method
    QDomNodeList testMethodLs = clientElm.elementsByTagName("testMethod");
    QString testMethod = testMethodLs.at(0).toElement().attribute("name");
    testMethodValue = testMethodLs.at(0).toElement().attribute("value").toInt();
    if (testMethod.compare("-t")){
        iTestMethod = testMethodType::TIMES;
    } else if(testMethod.compare("-n")){
        iTestMethod = testMethodType::BYTES;
    } else if(testMethod.compare("-k")){
        iTestMethod = testMethodType::BLOCKS;
    } else {
        iTestMethod = testMethodType::TIMES;
    }
    QDomNodeList omitLs = clientElm.elementsByTagName("omit");
    mOmit = omitLs.at(0).toElement().attribute("value").toInt();

    QDomNodeList bufferLs = clientElm.elementsByTagName("buffer");
    mBuffer = bufferLs.at(0).toElement().attribute("value").toInt();
    QDomNodeList windowLs = clientElm.elementsByTagName("window");
    mWindow = windowLs.at(0).toElement().attribute("value").toInt();
    QDomNodeList reverseLs = clientElm.elementsByTagName("reverse");
    mReverse = reverseLs.at(0).toElement().attribute("value").toInt();
    QDomNodeList bitrateLs = clientElm.elementsByTagName("bitrate");
    mBitrate = bitrateLs.at(0).toElement().attribute("value").toInt();

}

QString IperfXmlPair::getServerIP()
{
    return mServerIP;
}

int IperfXmlPair::getServerPort()
{
    return mServerPort;
}
