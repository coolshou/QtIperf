#include "iperfxmlcfg.h"

/*
 <pair>
    <client/>
 </pair>
*/
IperfXmlCfg::IperfXmlCfg(QObject *parent) :
    QObject(parent)
{
    mDoc = QDomDocument();
    mRoot = mDoc.createElement("pair");
    //default element
    defaultConfig();
    mDoc.appendChild(mRoot);
    //default client element
    mClient = mDoc.createElement("client");
    defaultClientConfig();
    mRoot.appendChild(mClient);
}
void IperfXmlCfg::defaultConfig() {
    //create a xml format document for iperf server/client
    /* <pair>
        <port/>
        <format/>
        <verbose/>
        <json/>
        <forceflush/>
        <debug/>
        <bind/>
       </pair>
          "--logfile":"",
    */
    QDomElement port = mDoc.createElement("port");
    port.setAttribute("name","-p");
    port.setAttribute("value",5201);
    //-f
    QDomElement format = mDoc.createElement("format");
    format.setAttribute("name","-f");
    format.setAttribute("value","k");
    //-V
    QDomElement verbose = mDoc.createElement("verbose");
    verbose.setAttribute("name","-V");
    verbose.setAttribute("value",1);
    //-J
    QDomElement json = mDoc.createElement("json");
    json.setAttribute("name","-J");
    json.setAttribute("value",false);
    //--forceflush
    QDomElement forceflush = mDoc.createElement("forceflush");
    forceflush.setAttribute("name","--forceflush");
    forceflush.setAttribute("value",true);
    //-d
    QDomElement debug = mDoc.createElement("debug");
    debug.setAttribute("name","-d");
    debug.setAttribute("value",false);
    //-B
    QDomElement bind = mDoc.createElement("bind");
    bind.setAttribute("name","-B");
    bind.setAttribute("value","");

    mRoot.appendChild(port);
    mRoot.appendChild(format);
    mRoot.appendChild(verbose);
    mRoot.appendChild(json);
    mRoot.appendChild(forceflush);
    mRoot.appendChild(debug);
    mRoot.appendChild(bind);
}
void IperfXmlCfg::defaultClientConfig(){
    /* <client>
        <ServerIP/>
        <testMode/>
        <testMethod/>
        <omit/>
        <buffer/>
        <window/>
        <reverse/>
        <bitrate/>
       </client>
    */
    QDomElement serverIP = mDoc.createElement("ServerIP");
    serverIP.setAttribute("name","-c");
    serverIP.setAttribute("value","127.0.0.1");
    //test Mode , TCP, UDP, SCTP
    QDomElement testMode = mDoc.createElement("testMode");
    testMode.setAttribute("name","TCP"); //TCP: "", UDP: -u, SCTP: --sctp
    testMode.setAttribute("value","");// "" , -u, --sctp
    //test Method, -t, -n, -k
    QDomElement testMethod = mDoc.createElement("testMethod");
    testMethod.setAttribute("name","-t");
    testMethod.setAttribute("value","10");
    //omit, //omit times in sec
    QDomElement omit = mDoc.createElement("omit");
    omit.setAttribute("name","-O");
    omit.setAttribute("value","0");
    //buffer size //0: TCP: 128K, UDP: 1460 or dynamic
    QDomElement buffer = mDoc.createElement("buffer");
    buffer.setAttribute("name","-l");
    buffer.setAttribute("value","0");
    //window size //0: not set windows size use default
    QDomElement window = mDoc.createElement("window");
    window.setAttribute("name","-w");
    window.setAttribute("value","0");
    //reverse //1: reverse, 0: not reverse
    QDomElement reverse = mDoc.createElement("reverse");
    reverse.setAttribute("name","-R");
    reverse.setAttribute("value",false);
    //bitrate  //target bitrate in bits/sec (0 for unlimited)
    //          (default 1 Mbit/sec for UDP, unlimited for TCP)
    QDomElement bitrate = mDoc.createElement("bitrate");
    bitrate.setAttribute("name","--bitrate");
    bitrate.setAttribute("value","0");

    mClient.appendChild(serverIP);
    mClient.appendChild(testMode);
    mClient.appendChild(testMethod);
    mClient.appendChild(omit);
    mClient.appendChild(buffer);
    mClient.appendChild(window);
    mClient.appendChild(reverse);
    mClient.appendChild(bitrate);
}
QString IperfXmlCfg::get_cfg(){
    return mDoc.toString();
}
QDomElement IperfXmlCfg::get_rootElm(){
    return mDoc.documentElement();
}

int IperfXmlCfg::set_ServerIP(QString ip)
{
    return set_ClientTagValue("ServerIP", ip);
}
int IperfXmlCfg::set_TestMode(testModeType testType)
{
    QDomNode d;
    QDomNodeList dLs = mClient.elementsByTagName("testMode");
    int n = dLs.count();
    if (n==1){
        d = dLs.item(0);
        switch (testType) {
        case testModeType::TCP:
            d.toElement().setAttribute("name", "TCP");
            d.toElement().setAttribute("value", "");
            break;
        case testModeType::UDP:
            d.toElement().setAttribute("name", "UDP");
            d.toElement().setAttribute("value", "-u");
            break;
        case testModeType::SCTP:
            d.toElement().setAttribute("name", "SCTP");
            d.toElement().setAttribute("value", "--sctp");
            break;
        }
    }
    return n;
}
int IperfXmlCfg::set_Reverse(bool bVal)
{
    QDomNode d;
    QDomNodeList dLs = mClient.elementsByTagName("reverse");
    int n = dLs.count();
    if (n==1){
        d = dLs.item(0);
        d.toElement().setAttribute("value", bVal);
    }
    return n;

}
int IperfXmlCfg::set_TestMethod(testMethodType tmType, QString val)
{
    return set_ClientTestMethod(tmType, val);
}
int IperfXmlCfg::set_TestMethod(testMethodType tmType, int val)
{
    return set_ClientTestMethod(tmType, QString::number(val));
}
int IperfXmlCfg::set_ClientTestMethod(testMethodType tmType, QString val)
{
    QString tm="-t";
    QString unit="";
    QDomNode d;
    QDomNodeList dLs = mClient.elementsByTagName("testMethod");
    int n = dLs.count();
    if (n==1){
        d = dLs.item(0);
        switch (tmType){
        case testMethodType::TIMES:
            tm="-t";
            break;
        case testMethodType::BYTES:
            tm="-n";
            unit="k";
            break;
        case testMethodType::BLOCKS:
            tm="-k";
            unit="k";
            break;

        }
        d.toElement().setAttribute("name", tm);
        d.toElement().setAttribute("value", val+unit);
    }
    return n;
}

int IperfXmlCfg::set_ClientTagValue(QString tag, QString val)
{
    QDomNode d;
    QDomNodeList dLs = mClient.elementsByTagName(tag);
    int n = dLs.count();
    if (n==1){
        d = dLs.item(0);
        d.toElement().setAttribute("value", val);
    }
    return n;

}
int IperfXmlCfg::set_TagValue(QString tag, QString val)
{
    QDomNode d;
    QDomNodeList dLs = mRoot.elementsByTagName(tag);
    int n = dLs.count();
    if (n==1){
        d = dLs.item(0);
        d.toElement().setAttribute("value", val);
    }
    return n;

}
