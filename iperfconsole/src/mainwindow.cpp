#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle(QTIPERF);
    pixmap_iperfconsole = QPixmap(":/images/iperfconsole.png");
    img_Tx = QPixmap(":/images/right.png");
    img_Rx = QPixmap(":/images/left.png");
    //https://stackoverflow.com/questions/15827886/set-default-alignment-for-cells-in-qtablewidget
    protoitem = new QTableWidgetItem();
    protoitem->setTextAlignment(Qt::AlignCenter);

    //linux: $HOME/.config/MySoft/Star Runner.conf
    //win: %APPDATA%\MySoft\Star Runner.ini
    cfg=new QSettings();
    //qDebug() << "config:" <<    cfg->fileName();
    //testCfg = new iperfCfg();
    testCfg = new TestCfg();
    connect(testCfg,SIGNAL(PairAdded(int, QString, QString, int, QList<QString>, QString, QString)),
            this, SLOT(onPairAdded(int, QString, QString, int, QList<QString>, QString, QString)));
    ui->setupUi(this);
    ui->twTestPair->setItemPrototype(protoitem);

#if USE_TREEWIDGET==0
    ui->twTestPairs->setVisible(false);
#else
    ui->twTestPair->setVisible(false);
#endif
    tpChart= new TPChart();
    chartView = new QChartView(tpChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->vLayoutChart->addWidget(chartView);

    this->SetActions();

    this->readSettings();
    init_systray();
    init_menu();
    this->onNewFile(false);
    //dialog of config pair
    //m_dlgPair = new dlgPair(ls, testCfg);
    //connect(this, SIGNAL(updateEndpointIPs(QMultiMap<QString,QString>)),
    //        m_dlgPair, SLOT(on_upadteEndpointIPs(QMultiMap<QString,QString>)));

    bcReceiver= new BroadcastReceiver();
    connect(bcReceiver, SIGNAL(updateEndpoint(QString,QString)),
            this, SLOT(on_updateEndpoint(QString,QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, tr("Exit"),
        QString(tr("Warning: Are you sure to exit?")),
        QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No) {
        event->ignore();  //ignore event, app keeps running
    }
    else if (button == QMessageBox::Yes) {
        this->writeSettings();
        event->accept();  //accept event, app exit
    }
}

void MainWindow::writeSettings()
{
    cfg->beginGroup("MainWindow");
    cfg->setValue("size", size());
    cfg->setValue("pos", pos());
    cfg->endGroup();
}

void MainWindow::readSettings()
{
    cfg->beginGroup("MainWindow");
    resize(cfg->value("size", QSize(400, 400)).toSize());
    move(cfg->value("pos", QPoint(200, 200)).toPoint());
    cfg->endGroup();
}

void MainWindow::SetActions()
{
    //file
    connect(ui->actionNew,SIGNAL(triggered(bool)),this,SLOT(onNewFile(bool)));
    //ui->actionOpen
    //ui->actionSave
    connect(ui->actionQuit,SIGNAL(triggered(bool)),qApp,SLOT(quit()));
    //edit
    //ui->actionCopy
    //ui->actionCut
    //ui->actionPaste
    //ui->actionDelete
    //pair
    connect(ui->actionAdd,SIGNAL(triggered(bool)),this,SLOT(onAdd(bool)));
    connect(ui->actionEdit,SIGNAL(triggered(bool)),this,SLOT(onEdit(bool)));
    //ui->actionswap
    //option
    //ui->actionConfig
    //help
    //ui->actionAbout
}
int MainWindow::getPairColumn(testPairColumn e)
{
    QVariant v2 = qVariantFromValue(e);
    int iCol = v2.toInt();
    return iCol;
}

//void MainWindow::onPairAdded(int idx, QString e1, QString e2, int iRun, QString iperfargs, QString m1, QString m2){
void MainWindow::onPairAdded(int idx, QString e1, QString e2, int iRun, QList<QString> iperfargs, QString m1, QString m2){
    //qDebug() << "TODO: onPairAdded, update table twTestPair" << endl;
#if USE_TREEWIDGET==1
    //treeWidget ============================================
    /*
    //if (idx > ui->twTestPairs->topLevelItemCount()) {
    //
    //}
    model->addPair(idx, e1,e2,iRun,iperfargs,m1,m2);

    ui->twTestPairs->show();
    //ui->twTestPairs->update();
    */
#endif
    //tableWidget ============================================
    if (idx> ui->twTestPair->rowCount()){
        ui->twTestPair->setRowCount(idx+1);
    }
    ui->twTestPair->insertRow(ui->twTestPair->rowCount());
    int iRow = ui->twTestPair->rowCount()-1;
    int iCol;
    //TODO pair in group

    //status
    iCol = getPairColumn(testPairColumn::status);
    QTableWidgetItem * itemStatus = protoitem->clone();
    itemStatus->setText("init");
    ui->twTestPair->setItem( iRow, iCol, itemStatus);
    //endpoint1
    iCol = getPairColumn(testPairColumn::endpoint1);
    QTableWidgetItem * itemE1 = protoitem->clone();
    itemE1->setText(e1);
    ui->twTestPair->setItem( iRow, iCol, itemE1);
    //TODO: direction
    QString d="";
    QPixmap img;
    int iDir = iperfargs.indexOf("-R");
    if (iDir>0){
        d="<=";
        img = img_Rx;
    } else {
        d="=>";
        img = img_Tx;
    }
    iCol = getPairColumn(testPairColumn::direction);
    QTableWidgetItem * itemDirection = protoitem->clone();
    itemDirection->setText(d);
    itemDirection->setIcon(QIcon(img));
    ui->twTestPair->setItem( iRow, iCol, itemDirection);

    //endpoint2
    iCol = getPairColumn(testPairColumn::endpoint2);
    QTableWidgetItem * itemE2 = protoitem->clone();
    itemE2->setText(e2);
    ui->twTestPair->setItem( iRow, iCol, itemE2);

    //protocal
    QString p="";
    if (iperfargs.indexOf("-u")>0){
        p="UDP";
    } else {
        p="TCP";
    }
    iCol = getPairColumn(testPairColumn::protocal);
    QTableWidgetItem * itemProtocal = protoitem->clone();
    itemProtocal->setText(p);
    ui->twTestPair->setItem( iRow, iCol, itemProtocal);
    //TODO commont
    Q_UNUSED(iRun);
    Q_UNUSED(m1);
    Q_UNUSED(m2);
}
void MainWindow::onNewFile(bool b)
{
    //user document folder
    QString d = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    d = d.append(QDir::separator()).append("new.ipf");
    //qDebug() << "doc:" <<d;
    debug(QString("doc: %1").arg(d));
    //
#if USE_TREEWIDGET==1
    model = new TreeModel();
    ui->twTestPairs->setModel(model);
    ui->twTestPairs->show();
#endif
    //TODO: read ipf file setting

    Q_UNUSED(b);
}
void MainWindow::onAdd(bool b)
{
    //int idx = ui->twTestPair->rowCount();
    int idx = testCfg->getPairCount();
    //Q_UNUSED(idx);
    QList<int> ls;
    ls.append(idx);
    //open add dialog
    m_dlgPair = new DlgPair(ls, testCfg);
    //dlgPair dlg(ls, testCfg);
    m_dlgPair->set_endpointIPs(m_mapEndpointIPs);
    connect(this, SIGNAL(updateEndpointIPs(QMultiMap<QString,QString>)),
            m_dlgPair, SLOT(on_upadteEndpointIPs(QMultiMap<QString,QString>)));
    //int dialogCode= dlg.exec();
    int dialogCode= m_dlgPair->exec();
    if(dialogCode == QDialog::Accepted) {
        //qDebug() << "Accepted";
        ui->actionStart->setEnabled(true);
    }
    //m_dlgPair->close();
    delete m_dlgPair;

    Q_UNUSED(b);

}
/*
void MainWindow::onDlgPairFinished(int result){ //this is a slot
   if(result == QDialog::Accepted){
       //do something
       return;
   }
   //do another thing
}
*/

void MainWindow::onEdit(bool b)
{
    //get selected pair
    //int idx = ui->twTestPair->selectedIndexes();

    //settings to dlgPair
    //qDebug() << "//TODO: onEdit";
    debug("TODO: onEdit");
    //open edit dialog
    //dlgPair *dlg = new dlgPair();
    //dlg->exec();
    Q_UNUSED(b);
}

void MainWindow::init_menu()
{
    menuTestPair = new QMenu(ui->twTestPair);
    menuTestPair->addAction(ui->actionCopy);
    menuTestPair->addAction(ui->actionPaste);
    menuTestPair->addAction(ui->actionCut);
    menuTestPair->addAction(ui->actionDelete);

    ui->twTestPair->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->twTestPair, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ProvideTestPairMenu(QPoint)));

}
void MainWindow::ProvideTestPairMenu(QPoint pos)
{
    menuTestPair->exec(ui->twTestPair->mapToGlobal(pos));
}
void MainWindow::init_systray()
{
    tray = new AniSysTrayIcon(QIcon(pixmap_iperfconsole),1, this);
    /*
    //# to test animate
    //# traymenu.addAction("start", lambda: self.tray.animate_start())
    //# traymenu.addAction("stop", lambda: self.tray.animate_stop())
    //# traymenu.addSeparator()
    //# traymenu.addAction("Open Web", lambda: open_new_tab(URL))
    //# traymenu.addAction("Quit All", lambda: self.close())
    */
    //connect(tray, SIGNAL(startSync()), this, SLOT(run()));
    //connect(tray, SIGNAL(stopSync()), this, SLOT(syncthing_stop()));
    connect(tray, SIGNAL(hideGUI()), this, SLOT(hide()));
    connect(tray, SIGNAL(showGUI()), this, SLOT(show_gui()));
    connect(tray, SIGNAL(exitGUI()), this, SLOT(app_exit()));
    tray->setVisible(true);
}

void MainWindow::app_exit()
{
    QString sInfo =  QString("Quit ").append(QTIPERF).append(" ?");
    bool the_conditional_is_true = QMessageBox::question(
        this, QTIPERF, sInfo,
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No) == QMessageBox::Yes;
    if (the_conditional_is_true){
        tray->stop_animate();
        exit(0);
    }else{
        //tray->setVisible(true);
    }
}
void MainWindow::show_gui()
{
    showNormal();
}

void MainWindow::on_actionCopy_triggered()
{
    //TODO: copy pair
    // get selected index
    QList<QTableWidgetSelectionRange> ls;
    ls= ui->twTestPair->selectedRanges();
    if (ls.length()>0){
        //foreach (QTableWidgetSelectionRange idx, ls) {
        //    qDebug() << "idx:" << idx.topRow() << " - " << idx.bottomRow();
        //}
        //qDebug() << "idx:" << ls.topRow() << " - " << ls.bottomRow();
        //qDebug() << "num:" << ls.count();
        debug(QString("TODO: copy pair num %1").arg(ls.count()));
    }
}

void MainWindow::on_actionPaste_triggered()
{
    //TODO: paste pair
    //qDebug() << "TODO: paste pair";
    debug("TODO: paste pair");
}

void MainWindow::on_twTestPair_itemSelectionChanged()
{
    bool bEnable=true;
    //TODO: twTestPair select something
    QList<QTableWidgetSelectionRange> ls;
    ls= ui->twTestPair->selectedRanges();
    if (ls.length()>0){
        bEnable=true;
    }else{
        bEnable=false;
    }
    ui->actionCopy->setEnabled(bEnable);
    ui->actionDelete->setEnabled(bEnable);
    ui->actionCut->setEnabled(bEnable);
    ui->actionSwap->setEnabled(bEnable);
}

void MainWindow::on_actionSwap_triggered()
{
    //TODO: swap pair;
    //qDebug() << "TODO: swap pair";
    debug("TODO: swap pair");
}

void MainWindow::on_updateEndpoint(QString hostname, QString json)
{
    QJsonDocument jDoc;
    QJsonObject jObj;
    QJsonArray jIPs;

    if (m_mapEndpoints.contains(hostname)){
        m_mapEndpoints[hostname] = json;
    }else{
        m_mapEndpoints.insert(hostname, json);
    }
    //update all endpoints IP list
    m_mapEndpointIPs.clear();
    foreach (QString e, m_mapEndpoints) {
        //qDebug() << "on_updateEndpoint:" << e;
        jDoc = QJsonDocument::fromJson(e.toUtf8());
        jObj =jDoc.object();
        QString hname = jObj["HOSTNAME"].toString();
        jIPs = jObj["IP"].toArray();
        foreach (QJsonValue Ip , jIPs) {
            //qDebug() << hname << " - IP: " << Ip.toString();
            debug(QString("%1 - IP: %2").arg(hname).arg(Ip.toString()));
            m_mapEndpointIPs.insert(hname, Ip.toString());
        }
    }
    emit updateEndpointIPs(m_mapEndpointIPs);
}

void MainWindow::stopEndpointClients()
{
    if (mEndpointClientsC.count()>0){
        //qDebug() << "stop old running iperf client" ;
        debug("stop old running iperf client");
        for (int i=0; i<mEndpointClientsC.count(); i++){
            EndpointClient *c = mEndpointClientsC.at(i);

            c->stopClient();
        }
    }
}

void MainWindow::stopEndpointServers()
{
    if (mEndpointClientsS.count()>0){
        //qDebug() << "stop old running iperf server" ;
        debug("stop old running iperf server");
        for (int i=0; i<mEndpointClientsS.count(); i++){
            EndpointClient *s = mEndpointClientsS.at(i);
            s->stopServer();
        }
    }
}

void MainWindow::on_actionStart_triggered()
{   //start running iperf
    bool allOK = true;
    setRunning(true);
    stopEndpointClients();
    stopEndpointServers();

    //qDebug() << "setup iperf client & server" ;
    debug("setup iperf client & server");
    mEndpointClientsC.clear();
    mEndpointClientsS.clear();
    //TODO: start running test
    //qDebug() << "test cfg: " << testCfg->toXMLString();
    QDomNodeList pLs = testCfg->get_PairElms();
    //qDebug() << "count:" << pLs.count();
    for (int idx=0; idx<pLs.count(); idx++){
        QString cfg;
        QTextStream stream(&cfg);
        QDomNode n = pLs.at(idx);
        n.save(stream,2);
        //qDebug() << "pair str: " << str;
        QDomElement elm = n.toElement();
        QString m2 = elm.attribute("managerIP2", "127.0.0.1");
        QString m1 = elm.attribute("managerIP1", "127.0.0.1");
        //foreach test pair
        //tell managerIP2 endpoint to add server
        //qDebug() << "create iperf server rpc" ;
        debug("create iperf server rpc");
        EndpointClient *epcs = new EndpointClient(qApp, cfg, m2);
        connect(epcs, SIGNAL(sig_error(int,QString)), this, SLOT(on_EndpointClientError(int,QString)));
        mEndpointClientsS.append(epcs);
        epcs->getIperfVersion();

        LastResult rs = epcs->getLastResult();
        if (rs.statue==0){
            //qDebug() << "ver:" << rs.result;
            debug(QString("ver: %1").arg(rs.result));
            //add server
            epcs->addServer();
            rs = epcs->getLastResult();
            if (rs.statue==0){
                int port = rs.result.toInt();
                //start server
                epcs->startServer();
                rs = epcs->getLastResult();
                if (rs.statue==0){
                    //server start ok
                    testCfg->setPairPort(idx, port);
                    //TODO: add client
                    //qDebug() << "add client at to port:" << port;
                    //qDebug() << "testcfg: \n" << testCfg->toXMLString();
                    //qDebug() << "\n======================================";
                    debug(QString("add client at to port: %1 testcfg: \n%2\n======================================")
                          .arg(port).arg(testCfg->toXMLString()));
                    QString ccfg = testCfg->get_PairString(idx);
                    //QDomNode nc = testCfg->get_PairElms(idx);
                    //nc.save(streamc,2);
                    EndpointClient *epcc = new EndpointClient(qApp, ccfg, m1);
                    connect(epcc, SIGNAL(sig_error(int,QString)), this, SLOT(on_EndpointClientError(int,QString)));
                    mEndpointClientsC.append(epcc);
                    //tell managerIP1 endpoint to add server
                    epcc->addClient();
                    rs = epcc->getLastResult();
                    if (rs.statue==0){
                        //add client ok
                        //epcc->startClient();
                    } else{
                        allOK = false;
                        break;
                    }
                } else{
                    allOK = false;
                    break;
                }
            }else{
                allOK = false;
                break;
            }
        } else {
            allOK = false;
            break;
        }
        QApplication::processEvents();

    }
    if (allOK){
        /*
        qDebug() << "run iperf server" ;
        for (int i=0; i<mEndpointClientsS.count(); i++){
            EndpointClient *s = mEndpointClientsS.at(i);
            s->startServer();
        }*/
        //qDebug() << "run all iperf client" ;
        debug("run all iperf client");
        for (int i=0; i<mEndpointClientsC.count(); i++){
            EndpointClient *c = mEndpointClientsC.at(i);
            //qDebug() << "start:" << i;
            debug(QString("start: %1").arg(i));
            c->startClient();
            QApplication::processEvents();
        }
    }
}
void MainWindow::on_EndpointClientError(int error, const QString &message)
{
    //qDebug() << "on_EndpointClientError:" << error << ":" << message;
    debug(QString("on_EndpointClientError: %1 : %2").arg(error).arg(message));
}

void MainWindow::setRunning(bool bStart){
    ui->actionStart->setEnabled(!bStart);
    ui->actionStop->setEnabled(bStart);

}

void MainWindow::on_actionStop_triggered()
{
    //TODO: stop all iperf client & server
    stopEndpointClients();
    stopEndpointServers();
    setRunning(false);

}
void MainWindow::debug(QString msg){
    QDateTime now = QDateTime::currentDateTime();
    QString strBuffer = now.toString("yyyy-MM-dd hh:mm:ss");

    qDebug() << "[" << strBuffer << "][MainWindow]" << msg;
}
