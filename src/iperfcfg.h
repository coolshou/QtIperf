#ifndef IPERFCFG_H
#define IPERFCFG_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

class iperfcfg : public QObject
{
    Q_OBJECT
    enum testModeType {
        TCP,
        UDP,
        SCTP
    };
public:
    explicit iperfcfg(QObject *parent = nullptr);
    void defaultConfig();
    void defaultClientConfig();
    //set
    int set_ServerIP(QString ip);
    int set_ServerPort(int port);
    int set_TestMode(testModeType tMode);
    //get
    QString get_cfgJSON();

signals:

public slots:

private:
    QJsonDocument jDoc;
    QJsonObject jCommObj;
    QJsonObject jClientObj;
};

#endif // IPERFCFG_H
