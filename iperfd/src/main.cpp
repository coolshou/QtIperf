#include <QCoreApplication>
#include <QTimer>
#include <QUrl>
#include <QSettings>
#include <QtService>
#include <QDir>

#include "iperfd.h"

#include <QDebug>

class IperfService : public QtService<QCoreApplication>
{
public:
    IperfService(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "Iperf Daemon")
    {
        setServiceDescription("IperfD service");
        setServiceFlags(QtServiceBase::CanBeSuspended);
    }

protected:
    void start()
    {
        QCoreApplication *app = application();
        app->setApplicationName("QtIperf");
/*
#if QT_VERSION < 0x040100
        quint16 port = (app->argc() > 1) ?
                QString::fromLocal8Bit(app->argv()[1]).toUShort() : 8080;
#else
        const QStringList arguments = QCoreApplication::arguments();
        quint16 port = (arguments.size() > 1) ?
                arguments.at(1).toUShort() : 8080;
#endif*/
        daemon = new IperfD(app);
        if (!daemon->start()) {
            logMessage(QString("Failed to start IperfD"), QtServiceBase::Error);
            app->quit();
        }
        /*
        if (!daemon->isListening()) {
            logMessage(QString("Failed to bind to port %1").arg(daemon->serverPort()), QtServiceBase::Error);
            app->quit();
        }
        */
    }

    void pause()
    {
    daemon->pause();
    }

    void resume()
    {
    daemon->resume();
    }

private:
    IperfD *daemon;
};

int main(int argc, char *argv[])
{
/* linux => tmp
#if !defined(Q_OS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
*/
    //QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    IperfService service(argc, argv);
    return service.exec();

    /*
    QCoreApplication app(argc, argv);
    QUrl APP_PATH(QString("%1").arg(app.applicationDirPath()));
    app.setProperty("APP_PATH",APP_PATH);

    IperfD daemon(&app);
    // Quit application when work is finished (emit signal finished)
    QObject::connect(&daemon, SIGNAL(finished()), &app, SLOT(quit())); // changed the
    QTimer::singleShot(0, &daemon, SLOT(start()));
    return app.exec();
    */
}
