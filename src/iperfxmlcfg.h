#ifndef IPERFXMLCFG_H
#define IPERFXMLCFG_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>

enum class testModeType {
    TCP,
    UDP,
    SCTP
};

enum class testMethodType {
    TIMES, //-t, time
    BYTES, //-n, bytes
    BLOCKS //-k, blocks
};

class IperfXmlCfg : public QObject
{
    Q_OBJECT
public:
    explicit IperfXmlCfg(QObject *parent = nullptr);
    //get
    QString get_cfg();
    QDomElement get_rootElm();
    //set
    void defaultConfig();
    void defaultClientConfig();
    int set_ServerIP(QString ip);
    int set_Reverse(bool bVal);
    int set_TestMode(testModeType testType);
    int set_TestMethod(testMethodType tmType, QString val);
    int set_TestMethod(testMethodType tmType, int val);

protected:
    int set_TagValue(QString tag, QString val);
    int set_ClientTagValue(QString tag, QString val);
    int set_ClientTestMethod(testMethodType tmType, QString val);

signals:

public slots:

private:
    QDomDocument mDoc;
    QDomElement mRoot;
    QDomElement mClient;
};

#endif // IPERFXMLCFG_H
