#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMetaEnum>
#include <QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCore/QTextStream>
#include <QDateTime>
#include <QTimer>
#include <QSettings>
#include <QCloseEvent>
#include <QTableWidgetItem>
#include <QMap>
#include <QMultiMap>

#include "../src/const.h"
#include "tpchart.h"
#include "anisystrayicon.h"
#include "dlgpair.h"
//#include "iperfcfg.h"
#include "testcfg.h"
#include "treemodel.h"
#include "broadcastreceiver.h"
#include "endpointclient.h"

#include <QDebug>

#define USE_TREEWIDGET 0

QT_CHARTS_USE_NAMESPACE

// c = class name; e = enum name; v = enum value
#define ENUM_TO_STRING(c, e, v) \
    (c::staticMetaObject.enumerator(c::staticMetaObject.indexOfEnumerator(#e)).valueToKey(v))
// c = class name; e = enum name; s = string value
#define STRING_TO_ENUM(c, e, s) \
    (c::e)(c::staticMetaObject.enumerator(c::staticMetaObject.indexOfEnumerator(#e)).keyToValue(s.toAscii()))
// c = class name; f = flag name, v = flag value
#define FLAG_TO_STRING(c, f, v) \
    (c::staticMetaObject.enumerator(c::staticMetaObject.indexOfEnumerator(#f)).valueToKeys(v))
// c = class name; f = flag name; s = string value
#define STRING_TO_FLAG(c, f, s) \
    (c::f)(c::staticMetaObject.enumerator(c::staticMetaObject.indexOfEnumerator(#f)).keysToValue(s.toAscii()))
//c = class name; e = enum name
#define ENUM_KEY_COUNT(c,e) \
    (c::staticMetaObject.enumerator(c::staticMetaObject.indexOfEnumerator(#e)).keyCount())

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    //Q_ENUMS(Priority)
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetActions();
    void writeSettings();
    void readSettings();
    void closeEvent(QCloseEvent *event);
    enum testPairColumn {
        group=0,
        status,
        endpoint1,
        direction,
        endpoint2,
        protocal,
        throughput,
        comment
    };
    enum PairDirection {
        Tx,
        Rx
    };
    void setRunning(bool start);
    void stopEndpointClients();
    void stopEndpointServers();

signals:
    void updateEndpointIPs(QMultiMap<QString, QString> mapIPs);

public slots:
    void app_exit();

private slots:
    void onNewFile(bool b);
    void onAdd(bool b);
    void onEdit(bool b);
    void show_gui();
    //void onPairAdded(int idx, QString e1, QString e2, int iRun, QString iperfargs, QString m1, QString m2);
    void onPairAdded(int idx, QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1, QString m2);
    void ProvideTestPairMenu(QPoint pos);
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_twTestPair_itemSelectionChanged();
    void on_actionSwap_triggered();
    void on_updateEndpoint(QString hostname, QString json);

    void on_actionStart_triggered();
    void on_EndpointClientError(int error, const QString &message);

    void on_actionStop_triggered();

private:
    void init_systray();
    void init_menu();
    int getPairColumn(testPairColumn e);

private:
    Ui::MainWindow *ui;
    DlgPair *m_dlgPair; //dialog for add/edit iperf test pair
    BroadcastReceiver *bcReceiver = nullptr;
    QMap<QString, QString> m_mapEndpoints; //hostname, json info of endpoint
    QMultiMap<QString, QString> m_mapEndpointIPs; //hostname, ip

    QList<EndpointClient*> mEndpointClientsS; //store each endpoint client to control iperf server
    QList<EndpointClient*> mEndpointClientsC; //store each endpoint client to control iperf client

    QStringList mEndpoints; //endpoint's ip address list
    QMenu *menuTestPair; //menu for TestPair
    QSettings *cfg; //app config
    TestCfg *testCfg; //test config (include each pair of iperf setting)
    //iperfCfg *testCfg; //test config (include each pair of iperf setting)
#if USE_TREEWIDGET==1
    //treemodel
    TreeModel *model; //hole tree's model data
#endif
    QPixmap pixmap_iperfconsole;
    QPixmap img_Tx;
    QPixmap img_Rx;

    AniSysTrayIcon *tray;
    QMenu *traymenu;
    //throughput chart
    TPChart *tpChart; //= new QChart();
    QChartView *chartView;
    QLineSeries *series;
    //QChart *chart; //= new QChart();
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    //
    QTableWidgetItem * protoitem;

    void debug(QString msg);
};
Q_DECLARE_METATYPE(MainWindow::testPairColumn)
Q_DECLARE_METATYPE(MainWindow::PairDirection)

#endif // MAINWINDOW_H
