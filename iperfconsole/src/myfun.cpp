#include "myfun.h"

myFun::myFun(QObject *parent) : QObject(parent)
{

}
bool myFun::isLinkLocalIPv4(QHostAddress addr){
    //check local IPv4 address
    //169.254.1.0 - 169.254.254.255
    //Qt 5.11 QHostAddress::isLinkLocal()
    quint32 nIPv4 = addr.toIPv4Address();
    quint32 nMinRange = QHostAddress("169.254.1.0").toIPv4Address();
    quint32 nMaxRange = QHostAddress("169.254.254.255").toIPv4Address();
    if ((nIPv4 >= nMinRange) && (nIPv4 <= nMaxRange)) {
        return true;
    } else {
        return false;
    }
}

bool myFun::isLocalIPv4(QHostAddress addr){
    //check private IPv4 address
    //10.0.0.0 - 10.255.255.255
    //172.16.0.0 - 172.31.255.255
    //192.168.0.0 - 192.168.255.255

    quint32 nIPv4 = addr.toIPv4Address();
    quint32 nMinRange1 = QHostAddress("10.0.0.0").toIPv4Address();
    quint32 nMaxRange1 = QHostAddress("10.255.255.255").toIPv4Address();

    quint32 nMinRange2 = QHostAddress("172.16.0.0").toIPv4Address();
    quint32 nMaxRange2 = QHostAddress("172.31.255.255").toIPv4Address();

    quint32 nMinRange3 = QHostAddress("192.168.0.0").toIPv4Address();
    quint32 nMaxRange3 = QHostAddress("192.168.255.255").toIPv4Address();

    if ((nIPv4 >= nMinRange1 && nIPv4 <= nMaxRange1)
            || (nIPv4 >= nMinRange2 && nIPv4 <= nMaxRange2)
            || (nIPv4 >= nMinRange3 && nIPv4 <= nMaxRange3)) {
        return true;
    } else {
        return false;
    }
}
bool isVaildIPv4(QHostAddress addr){
    //TODO: check is vaild IPv4 Address
    //https://en.wikipedia.org/wiki/IPv4
    //0.0.0.0 ~ 255.255.255.255
    Q_UNUSED(addr);
    return false;
}
