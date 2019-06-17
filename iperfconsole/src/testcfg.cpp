#include "testcfg.h"

#include <QDomNodeList>
#include <QRegularExpression>

#include <QDebug>
/*
<setting>
    <pairs>
        <pair></pair> //iperfxmlcfg
        <pair></pair>
        <pair></pair>
    </pairs>
</setting>


*/
TestCfg::TestCfg(QObject *parent) : QObject(parent)
{
    mDoc = QDomDocument();
    mRoot = mDoc.createElement("setting");
    //default element
    //defaultConfig();
    mDoc.appendChild(mRoot);
    //default client element
    mPairs = mDoc.createElement("pairs");
    //defaultClientConfig();
    mRoot.appendChild(mPairs);
}
int TestCfg::addPair(QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1, QString m2){
    //TODO: add iperf Pair
    int idx;
    idx = getPairCount();

    IperfXmlCfg pair; //default iperf setting
    QDomElement pairElm = pair.get_rootElm();
    pairElm.setAttribute("idx",idx);
    pairElm.setAttribute("endpoint1",e1);
    pair.set_ServerIP(e2);
    pairElm.setAttribute("endpoint2",e2);
    //iTime
    //TODO: detail setting
    QString t;
    for (int i = 0; i < iperfargs.count(); ++i) {
        //iperfarg = iperfarg+" "+ iperfargs.value(i);
        t = iperfargs.value(i);
        qDebug() << "iperf args:" << t;
        debug(QString("iperf args:%1").arg(t));
        if (t.contains("-t")){
            pair.set_TestMethod(testMethodType::TIMES, t.split(" ")[1]);
        }
        if (t.contains("-n")){
            pair.set_TestMethod(testMethodType::BYTES, t.split(" ")[1]);
        }
        if (t.contains("-k")){
            pair.set_TestMethod(testMethodType::BLOCKS, t.split(" ")[1]);
        }
        if (t.contains("-u")){
            pair.set_TestMode(testModeType::UDP);
        }
        if (t.contains("-sctp")){
            pair.set_TestMode(testModeType::SCTP);
        }
        if (t.contains("-R")){
            pair.set_Reverse(true);
        }
        //TODO: many iperf config
    }
    pairElm.setAttribute("managerIP1",m1);
    pairElm.setAttribute("managerIP2",m2);

    //qDebug() << "addPair"<< pair.get_cfg();
    mPairs.appendChild(pairElm);

    emit PairAdded(idx, e1, e2, iRun, iperfargs, m1, m2);
    return idx+1;
}

int TestCfg::getPairCount(){
    QDomNodeList nodelist = mPairs.elementsByTagName("pair");
    return nodelist.count();;
}
QDomNodeList TestCfg::get_PairElms(){
    //return pair element list
    return mPairs.elementsByTagName("pair");
}
QString TestCfg::get_PairString(int idx)
{
    QString cfg="";
    QTextStream stream(&cfg);
    QDomNodeList nLs = get_PairElms();
    int iMax = nLs.count();
    if (idx>=iMax){
        return cfg;
    }
    QDomNode n = nLs.at(idx);
    n.save(stream,2);
    return cfg;
}
int TestCfg::setPairPort(int idx, int port)
{
    QDomNodeList nLs = get_PairElms();
    int iMax =  nLs.count();
    if (idx>=iMax){
        return 0;
    }
    QDomNode n = nLs.at(idx);
    QDomElement elm = n.toElement();
    QDomNodeList nPort = elm.elementsByTagName("port");
    QDomElement elmPort = nPort.at(0).toElement();
    //qDebug() << "port:" << port;
    debug(QString("port:%1").arg(port));
    elmPort.setAttribute("value", port);
    return port;
}

void TestCfg::debug(QString msg)
{
    QDateTime now = QDateTime::currentDateTime();
    QString strBuffer = now.toString("yyyy-MM-dd hh:mm:ss");

    qDebug() << "[" << strBuffer << "][TestCfg]" << msg;
}
QString TestCfg::toXMLString(){
    return mDoc.toString();
}
QDomElement TestCfg::get_rootElm(){
    return mDoc.documentElement();
}
