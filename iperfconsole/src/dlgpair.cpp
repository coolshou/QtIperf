#include "dlgpair.h"
#include "ui_dlgpair.h"

#include <QHostAddress>

DlgPair::DlgPair(QList<int> idxs, TestCfg *cfg, QWidget *parent) :
    QDialog(parent), ui(new Ui::dlgPair)
{
    mSettings=new QSettings();
    ui->setupUi(this);
    connect(ui->pbOK, SIGNAL(clicked(bool)), this, SLOT(checkSetting(bool)));
    connect(ui->pbCancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
    oldComboStyle = ui->cbEndpoint1->styleSheet();

    if (idxs.length()==1){
        setWindowTitle(QString("Add Pair: %1").arg(idxs[0]));
        mNew=true;
    } else {
        //TODO: edit multi pair
        setWindowTitle(QString("Edit Pair"));
        mNew=false;
    }
    mCfg = cfg;
    readSettings();
}

DlgPair::~DlgPair()
{
    delete ui;
}
void DlgPair::set_endpointIPs(QMultiMap<QString, QString> mapIPs){
    m_mapIPs = mapIPs;
    QString s;
    for (QMultiMap<QString, QString>::const_iterator it = m_mapIPs.cbegin(), end = m_mapIPs.cend(); it != end; ++it) {
        //qDebug() << "The key: " << it.key();
        //qDebug() << "The value: " << it.value();
        s=QString("%1(%2)").arg(it.key()).arg(it.value());
        ui->cbEndpoint1->addItem(s,it.value());
        ui->cbEndpoint2->addItem(s,it.value());
        ui->cbManagerIP1->addItem(s,it.value());
        ui->cbManagerIP2->addItem(s,it.value());
    }
}
void DlgPair::readSettings()
{
    ui->cbEndpoint1->clear();
    ui->cbEndpoint2->clear();
    ui->cbManagerIP1->clear();
    ui->cbManagerIP2->clear();
    if (mNew){
        ui->cbEndpoint1->addItem("");
        ui->cbEndpoint2->addItem("");
        ui->cbManagerIP1->addItem("");
        ui->cbManagerIP2->addItem("");
    }
    //history of endpoint1, endpoint2
    mSettings->beginGroup("endpoint");
    QString ip;
    QStringList hisEPs = mSettings->childKeys();
    if (hisEPs.length()>0){
        foreach (QString k,hisEPs){
            ip = mSettings->value(k).toString();
            ui->cbEndpoint1->addItem(ip,ip);
            ui->cbEndpoint2->addItem(ip,ip);
            ui->cbManagerIP1->addItem(ip,ip);
            ui->cbManagerIP2->addItem(ip,ip);
        }
    }
    mSettings->endGroup();
}
void DlgPair::writeSettings(){
    //record endpoint1, endpoint2 for history use
    QString e1="",e2="";
    bool bE1, bE2;

    bE1=false;
    bE2=false;
    mSettings->beginGroup("endpoint");
    e1= ui->cbEndpoint1->currentText();
    e2= ui->cbEndpoint2->currentText();
    QStringList hisEPs = mSettings->childKeys();
    if (hisEPs.length()>0){
        foreach (QString k,hisEPs){
            if (e1.compare(mSettings->value(k).toString())==0){
                //found IP exist
                bE1=true;
            }
            if ((e2.compare(e1)==0)||(e2.compare(mSettings->value(k).toString())==0)) {
                bE2=true;
            }
        }
    }

    if ((! bE1) & (e1.length()>0)){
        int idx = hisEPs.length();
        mSettings->setValue(QString(idx), e1);
        hisEPs = mSettings->childKeys();
    }
    if ((! bE2) & (e2.length()>0)){
        int idx = hisEPs.length();
        mSettings->setValue(QString(idx), e2);
    }
    mSettings->endGroup();
/*
    //history of endpoint2
    bE2=false;
    cfg->beginGroup("endpoint2");
    e2= ui->cbEndpoint2->currentText();
    QStringList hisEP2s = cfg->childKeys();
    if (hisEP2s.length()>0){
        foreach (QString k,hisEP2s){
            if (e2 == cfg->value(k)){
                bE2=true;
            }
        }
    }
    if (! bE2){
        int idx = hisEP2s.length();
        cfg->setValue(QString(idx), e2);
    }
    cfg->endGroup();*/
}

void DlgPair::checkSetting(bool checked){
    Q_UNUSED(checked);
    if (ui->cbEndpoint1->currentText().length()<0){
        ui->cbEndpoint1->setFocus();
        ui->cbEndpoint1->setStyleSheet(errComboStyle);
        return;
    }
    if (ui->cbEndpoint2->currentText().length()<0){
        ui->cbEndpoint2->setFocus();
        ui->cbEndpoint2->setStyleSheet(errComboStyle);
        return;
    }

    setPairCfg();
    accept();
}
void DlgPair::setPairCfg(){
    //store all settings to json?
    QList<QString> iperfArgs;
    //QString args;
    QString e1,e2;
    QString m1,m2;
    e1= ui->cbEndpoint1->currentText();
    e2= ui->cbEndpoint2->currentText();
    //TODO: check e1, e2 is IPv4 or IPv6
    if (ui->ckManagerIP1->isChecked()){
        m1= e1;
    }else{
        m1= ui->cbManagerIP1->currentText();
    }
    if (ui->ckManagerIP2->isChecked()){
        m2= e2;
    }else{
        m2= ui->cbManagerIP2->currentText();
    }

    int iRun;
    if (ui->rbTime->isChecked()){
        //run time
        iRun = ui->sbTime->value();
        iperfArgs.append(QString("-t %1").arg(iRun));
        //args.append("-t %1").arg(iRun);
    } else {
        iRun = 0;
        qDebug() << "//TODO: --bytes, --blockcount";
    }
    if (ui->ckReverse->isChecked()){
        iperfArgs.append(QString("-R"));
    }
    mCfg->addPair(e1, e2, iRun, iperfArgs, m1, m2);

    writeSettings();
    QDialog::accept();
}
void DlgPair::onCancelButtonClicked(){
    this->setResult(QDialog::Rejected);
    this->close();
}

void DlgPair::on_ckManagerIP1_stateChanged(int state)
{
    if (state == Qt::Checked){
        ui->cbManagerIP1->setEnabled(false);
    } else{
        ui->cbManagerIP1->setEnabled(true);
    }
}

void DlgPair::on_ckManagerIP2_stateChanged(int state)
{
    if (state == Qt::Checked){
        ui->cbManagerIP2->setEnabled(false);
    } else{
        ui->cbManagerIP2->setEnabled(true);
    }
}
bool DlgPair::isVaildAddress(QString &addr)
{   //TODO: more error address check!!
    bool bRS=true;
    QHostAddress haddr;
    if (! haddr.setAddress(addr)){
        bRS = false;
    }else{
        if (haddr.isNull()){
            //null
            bRS=false;
        }
        if (haddr.isEqual(QHostAddress::Any)){
            //0.0.0.0 or ::
            bRS= false;
        }
        if (haddr.isEqual(QHostAddress::Broadcast)){
            //255.255.255.255
            bRS= false;
        }
        if (haddr.protocol()==QAbstractSocket::IPv4Protocol){
            // correct format of string xxx.xxx.xxx.xxx
            int iNum =  addr.count(".");
            if (iNum<3){
                bRS=false;
            } else {
                QStringList s = addr.split(".");
                if ((s[3]=="0")||(s[3]=="255")) {
                    //subnet x.x.x.0
                    //Broadcast x.x.x.255
                    bRS=false;
                }
            }
        }else if(haddr.protocol()==QAbstractSocket::IPv6Protocol){
            QStringList IPV6List("");
            Q_IPV6ADDR IPV6 = haddr.toIPv6Address();
            for (int i = 0; i < 16; ++i) {
                       quint8 nC = IPV6[i];
                       IPV6List << QString::number(nC);
            }
            qDebug() << "IPv6:" << IPV6List.join(" ");
        }
    }
    if (bRS){
        addr = haddr.toString();
    }
    return bRS;
}
void DlgPair::on_cbEndpoint1_currentIndexChanged(int index)
{
    QString ip = ui->cbEndpoint1->itemData(index).toString();
    if (isVaildAddress(ip)){
        ui->cbEndpoint1->setCurrentText(ip);
        ui->cbEndpoint1->setStyleSheet(oldComboStyle);
    } else {
        ui->cbEndpoint1->setFocus();
        //set color
        ui->cbEndpoint1->setStyleSheet(errComboStyle);
    }
}
void DlgPair::on_cbEndpoint2_currentIndexChanged(int index)
{
    QString ip = ui->cbEndpoint2->itemData(index).toString();
    if (isVaildAddress(ip)){
        ui->cbEndpoint2->setCurrentText(ip);
        ui->cbEndpoint2->setStyleSheet(oldComboStyle);
    } else {
        ui->cbEndpoint2->setFocus();
        //set color
        ui->cbEndpoint2->setStyleSheet(errComboStyle);
    }
}

/*
void DlgPair::on_cbEndpoint1_currentTextChanged(const QString &arg1)
{
    QString ip = arg1;
    if (isVaildAddress(ip)){
        ui->cbEndpoint1->setCurrentText(ip);
        ui->cbEndpoint1->setStyleSheet(oldComboStyle);
    } else {
        ui->cbEndpoint1->setFocus();
        //set color
        ui->cbEndpoint1->setStyleSheet(errComboStyle);
    }
}

void DlgPair::on_cbEndpoint2_currentTextChanged(const QString &arg1)
{
    QString ip = arg1;
    if (isVaildAddress(ip)){
        ui->cbEndpoint2->setCurrentText(ip);
        ui->cbEndpoint2->setStyleSheet(oldComboStyle);
    } else {
        ui->cbEndpoint2->setFocus();
        //set color
        ui->cbEndpoint2->setStyleSheet(errComboStyle);
    }
}*/
//
void DlgPair::on_upadteEndpointIPs(QMultiMap<QString, QString> mapIPs)
{
    set_endpointIPs(mapIPs);
}






