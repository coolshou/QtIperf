#ifndef DLGPAIR_H
#define DLGPAIR_H

#include <QDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QAbstractButton>
#include <QCloseEvent>
#include <QStyle>
#include <QSettings>
#include <QMultiMap>

#include "testcfg.h"

namespace Ui {
class dlgPair;
}

class DlgPair : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPair(QList<int> idxs, TestCfg *cfg, QWidget *parent = 0);
    ~DlgPair();
    void set_endpointIPs(QMultiMap<QString, QString> mapIPs);

public slots:
    void on_upadteEndpointIPs(QMultiMap<QString, QString> mapIPs);
private:
    void readSettings();
    void writeSettings();
    bool isVaildAddress(QString &addr);

private slots:
    void checkSetting(bool checked);
    void setPairCfg();
    void onCancelButtonClicked();
    void on_ckManagerIP1_stateChanged(int state);
    void on_ckManagerIP2_stateChanged(int state);

    //void on_cbEndpoint1_currentTextChanged(const QString &arg1);
    //void on_cbEndpoint2_currentTextChanged(const QString &arg1);

    void on_cbEndpoint1_currentIndexChanged(int index);
    void on_cbEndpoint2_currentIndexChanged(int index);

private:
    Ui::dlgPair *ui;
    QSettings *mSettings; //app config
    TestCfg *mCfg;
    bool mNew; //is add pair
    QMultiMap<QString, QString> m_mapIPs;
    QString oldComboStyle;
    QString errComboStyle="QComboBox:focus{ border: 3px solid red; }";
};

#endif // DLGPAIR_H
