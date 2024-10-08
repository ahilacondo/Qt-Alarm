#ifndef FILEIO_H
#define FILEIO_H

#include "schedulecollection.h"
#include "schedule.h"
#include <QObject>
#include <QSettings>
#include <QList>

class FileIO : public QObject
{
    Q_OBJECT
public:
    explicit FileIO(QObject *parent = 0);
    QList<Schedule *> LoadConfig();
    bool Save(ScheduleCollection *);
    static int LoadVolume();
    static void SaveVolume(int);

private:
    QSettings _Settings;
    QList<Schedule *> LegacyRead();
signals:

public slots:
};

#endif // FILEIO_H
