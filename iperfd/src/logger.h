#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent , QString fileName);
    ~Logger();
     void setShowDateTime(bool value);

signals:

public slots:
 void write(const QString &value);

private:
 QFile *file;
 //QPlainTextEdit *m_editor;
 bool m_showDate;

};

#endif // LOGGER_H
