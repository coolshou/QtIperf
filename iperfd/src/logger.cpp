#include "logger.h"

#include <QTextStream>
#include <QDateTime>
#include <QDir>

#include <QDebug>

Logger::Logger(QObject *parent, QString fileName) : QObject(parent)
{
    QDir d = QDir(fileName);
    if (!d.exists()) {
        //make dir first
        QString path = QFileInfo(fileName).path();
        d.mkpath(path);
    }
    m_showDate = true;
     if (!fileName.isEmpty()) {
      file = new QFile;
      file->setFileName(fileName);
      file->open(QIODevice::Append | QIODevice::Text);
     }
}
void Logger::write(const QString &value) {
 QString text = value;// + "";
 if (m_showDate)
  text = QDateTime::currentDateTime().toString("[yyyy.MM.dd_hh:mm:ss] ") + text+"\n";
 QTextStream out(file);
 out.setCodec("UTF-8");
 if (file != 0) {
  out << text;
 }
 qDebug() << text;

}

void Logger::setShowDateTime(bool value) {
 m_showDate = value;
}

Logger::~Logger() {
 if (file != 0)
 file->close();
}
