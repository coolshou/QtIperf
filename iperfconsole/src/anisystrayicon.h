#ifndef ANISYSTRAYICON_H
#define ANISYSTRAYICON_H

#include <QObject>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QList>
#include <QIcon>
#include <QMenu>
#include <QTransform>

class AniSysTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    AniSysTrayIcon(const QIcon &icon, int interval=1, QWidget *parent = 0);
    void add_ani_icon(QIcon icon);
    void restore_icon();
    void start_animate();
    void stop_animate();
    void schedule_timer(int interval);
    void setShowHideMenu(bool bShow);

public slots:
    void onAniTimerTimeout();
    void doShowGUI();
    void doHideGUI();
    void trayExit();
signals:
    void showGUI();
    void hideGUI();
    void exitGUI();

protected:
    void update_trayicon();
private:
    QIcon mainicon;
    QPixmap aniIcon0;
    QPixmap aniIcon1;
    QPixmap aniIcon2;
    QPixmap aniIcon3;
    QPixmap aniIcon4;
    QAction *actShow;
    QAction *actHide;
    QTransform tf;
    QTimer *ani_timer;
    bool ani_stop;
    QList<QIcon> ani_icons;
    int ani_idx;
    int _interval;
    QMenu *traymenu;
};

#endif // ANISYSTRAYICON_H
