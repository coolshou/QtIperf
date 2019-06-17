#ifndef IPERFCFG_H
#define IPERFCFG_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include <QDebug>

/*
 * {
 *  "comment":""
 *  {
 *      "endpoint1": "192.168.0.10"
 *      "endpoint2": "192.168.0.20"
 *      "runtime": 10
 *      "managerIP1": ""
 *      "managerIP2": ""
 *      "iperfArgs":""
 *  }
 * }
 *
 *
 *
 *
 *
*/

class iperfCfg : public QObject
{
    Q_OBJECT
public:
    explicit iperfCfg(QObject *parent = nullptr);
    QString toJsonString();
    //int addPair(QString e1, QString e2, int iRun, QString iperfargs, QString m1="", QString m2="");
    int addPair(QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1="", QString m2="");
    int getPairCount();
signals:
    //void PairAdded(int idx, QString e1, QString e2, int iRun, QString iperfargs, QString m1="", QString m2="");
    void PairAdded(int idx, QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1="", QString m2="");

public slots:

private:
    int mIdx;
    QJsonObject * mJsonRoot;
    QJsonArray mJsonPairs;
    QFile * mFile; //file to store/load
    //QJsonObject * mIperf;
    //QString mEndpoint1;
    //QString mEndpoint2;
    //int     mRuntime;
    //QString mManagerIP1;
    //QString mManagerIP2;
    //
    QString mArgs; //iperf args
};

#endif // IPERFCFG_H
