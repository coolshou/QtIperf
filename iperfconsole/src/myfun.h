#ifndef MYFUN_H
#define MYFUN_H

#include <QObject>
#include <QHostAddress>

class myFun : public QObject
{
    Q_OBJECT
public:
    explicit myFun(QObject *parent = nullptr);
    bool isLinkLocalIPv4(QHostAddress addr);
    bool isLocalIPv4(QHostAddress addr);

signals:

public slots:
};

#endif // MYFUN_H
