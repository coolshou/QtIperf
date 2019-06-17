#include "anisystrayicon.h"
#include <QVariant>

#include "../src/const.h"

#include <QDebug>


AniSysTrayIcon::AniSysTrayIcon(const QIcon &icon, int interval, QWidget *parent)
{
    this->setParent(parent);
    this->mainicon = icon;
    tf = QTransform();
    aniIcon0 = icon.pixmap(icon.actualSize(QSize(32, 32))); //QPixmap(icon);
    aniIcon1 = icon.pixmap(icon.actualSize(QSize(32, 32))); //QPixmap(icon);
    tf.rotate(90.0);
    aniIcon2 = aniIcon0.transformed(tf);
    tf.rotate(180.0);
    aniIcon3 = aniIcon0.transformed(tf);
    tf.rotate(270.0);
    aniIcon4 = aniIcon0.transformed(tf);

    setIcon(icon);
    ani_timer = new QTimer(this);
    ani_timer->setInterval(interval*1000);
    ani_stop = false;
    ani_idx = 0;
    _interval = interval;
    //menu
    traymenu = new QMenu(parent);
    traymenu->addAction(QTIPERF)->setDisabled(true);
    //traymenu->addSeparator();
    //traymenu->addAction("Stop Sync", this ,SLOT(stop()));
    //traymenu->addAction("Restart Sync", this ,SLOT(start()));
    traymenu->addSeparator();
    actShow = traymenu->addAction("Show", this ,SLOT(doShowGUI()));
    actHide = traymenu->addAction("Hide", this ,SLOT(doHideGUI()));
    traymenu->addSeparator();
    //# traymenu.addAction("Open Web", lambda: open_new_tab(URL))
    //# traymenu.addAction("Quit All", lambda: self.close())
    traymenu->addAction("Quit All", this ,SLOT(trayExit()));

    //traymenu->show();
    setContextMenu(traymenu);
}

void AniSysTrayIcon::add_ani_icon(QIcon icon)
{
    this->ani_icons.append(icon);
}

void AniSysTrayIcon::restore_icon()
{
    ani_idx = 0;
    setIcon(mainicon);
}

void AniSysTrayIcon::start_animate()
{
    int total = ani_icons.length();

    if (total < 2){
        qDebug() << "Too few icons to do animate!!";
        /*
        try
            raise NameError("Too few icons to do animate!!");
        finally:
            raise;
            */
        return;
    }
    schedule_timer(_interval);
}
void AniSysTrayIcon::setShowHideMenu(bool bShow)
{
    actShow->setEnabled(!bShow);
    actHide->setEnabled(bShow);
}
void AniSysTrayIcon::stop_animate()
{
    ani_stop = true;
    if (ani_timer->isActive())
        ani_timer->stop();
    restore_icon();
}

void AniSysTrayIcon::schedule_timer(int interval)
{
    if (!ani_timer)
            ani_timer = new QTimer(this);
    ani_timer->setInterval(interval);
    connect(ani_timer,SIGNAL(timeout()), this, SLOT(onAniTimerTimeout()));
}

void AniSysTrayIcon::onAniTimerTimeout()
{
    update_trayicon();
    if (! ani_stop)
        schedule_timer(_interval);
}

void AniSysTrayIcon::doShowGUI()
{
    setShowHideMenu(true);
    emit showGUI();
}

void AniSysTrayIcon::doHideGUI()
{
    setShowHideMenu(false);
    emit hideGUI();
}

void AniSysTrayIcon::trayExit()
{
    //setVisible(false);
    emit exitGUI();
}

void AniSysTrayIcon::update_trayicon()
{
    int total = ani_icons.length();

    ani_idx = ani_idx + 1;
    //# print("ani_idx: %s" % self.ani_idx)
    if (ani_idx > (total - 1))
        ani_idx = 0;
    QIcon icon = ani_icons[ani_idx];
    setIcon(icon);
}
