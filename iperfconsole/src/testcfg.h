#ifndef TESTCFG_H
#define TESTCFG_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QDateTime>

#include "../src/iperfxmlcfg.h"

class TestCfg : public QObject
{
    Q_OBJECT
public:
    explicit TestCfg(QObject *parent = nullptr);
    int addPair(QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1="", QString m2="");
    int getPairCount();
    QDomNodeList get_PairElms();
    QString get_PairString(int idx);
    QString toXMLString();
    QDomElement get_rootElm();
    int setPairPort(int idx, int port);

signals:
    void PairAdded(int idx, QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1="", QString m2="");

public slots:

private:
    QDomDocument mDoc;
    QDomElement mRoot; //<setting>
    QDomElement mPairs; //<pairs>
    void debug(QString msg);
};

#endif // TESTCFG_H
