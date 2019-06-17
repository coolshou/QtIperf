#include "iperfcfg.h"
#include <QJsonDocument>
#include <iterator>
#include <QVariantMap>
#include <QVariantList>

iperfCfg::iperfCfg(QObject *parent):
    QObject(parent)
{
    //mIdx = idx;
    mJsonRoot=new QJsonObject();
    mJsonRoot->insert("Version", "1.0");
    QJsonArray mJsonPairs;
    mJsonRoot->insert("iperfs", mJsonPairs);
}
QString iperfCfg::toJsonString()
{
    QJsonDocument doc;
    doc.setObject(*mJsonRoot);
    if(doc.isNull()){
        qDebug()<<"Failed to create JSON doc.";
        exit(2);
    }

    QString strJson(doc.toJson(QJsonDocument::Compact));
    return strJson;
}

//return idx of pair
//int iperfCfg::addPair(QString e1, QString e2, int iRun, QString iperfargs, QString m1, QString m2){
int iperfCfg::addPair(QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1, QString m2){
    //TODO: add iperf Pair
    int idx;
    idx = getPairCount();
    QJsonObject pair;
    pair.insert("idx",idx);
    pair.insert("endpoint1",e1);
    pair.insert("endpoint2",e2);
    pair.insert("runtime",iRun);
    QString iperfarg="";
    for (int i = 0; i < iperfargs.count(); ++i) {
        iperfarg = iperfarg+" "+ iperfargs.value(i);
    }
    pair.insert("iperfArgs",iperfarg);
    pair.insert("managerIP1",m1);
    pair.insert("managerIP2",m2);
    mJsonPairs.append(pair);
    emit PairAdded(idx, e1, e2, iRun, iperfargs, m1, m2);
    return idx+1;
}


int iperfCfg::getPairCount(){
    return mJsonPairs.count();
    /*
    QVariantMap root_map = mJsonRoot->toVariantMap();
    QVariantMap iperf_map = root_map["iperf"].toMap();
    QVariantList iperf_list = root_map["iperf"].toList();
    int iNum = iperf_list.length();
    qDebug() << "iperf_list num: " << iNum;

    //TODO: get total iperf pairs number
   QJsonObject::iterator ji;
   ji = mJsonRoot->find("iperf");
//   int iNum=0;

   //if (ji == QJsonObject::end()){
   //    return 0;
   //}
//   QJsonObject::iterator ji;
   for(it=repData.begin();it!=repData.end();it++)
   {

   }
*/
}
