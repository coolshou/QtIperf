#ifndef IPERFXMLPAIR_H
#define IPERFXMLPAIR_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomNodeList>

#include "../src/const.h"
#include "../src/iperfxmlcfg.h"

class IperfXmlPair : public QObject
{
    Q_OBJECT
public:
    explicit IperfXmlPair(QString xmlCfg, QObject *parent = nullptr);
    QString getServerIP();
    int getServerPort();

signals:

public slots:
private:
    QString mServerIP;
    int mServerPort;
    //
    QString mFormat;
    testModeType iTestMode;
    testMethodType iTestMethod;
    int testMethodValue;
    int mOmit;
    int mBuffer;
    int mWindow;
    bool mReverse;
    int mBitrate;

};

#endif // IPERFXMLPAIR_H
