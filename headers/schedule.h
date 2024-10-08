#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QTime>
#include <QDateTime>
#include <QString>

class Schedule : public QObject
{
    Q_OBJECT
public:
    explicit Schedule(QObject *parent = 0);
    void SetCustEnabled(bool);
    void SetTime(QTime);
    void SetCust(QDate);
    void SetCustomSound(QString);
    void SetCustomSoundEnabled(bool);

    bool GetCustomEnabled();
    bool GetCustomSoundEnabled();

    QTime GetTime();
    QDate GetCustomDate();
    QString GetCustomSound();
    int Index;

    bool isCustomEnabled() const;
    void setIsCustomEnabled(bool isCustomEnabled);

    bool isCustomSoundEnabled() const;
    void setIsCustomSoundEnabled(bool isCustomSoundEnabled);

    QString Name();

private:
    QTime _AlarmTime;
    QDate _CustomAlarm;
    QString _CustomSound;
    bool _isCustomEnabled;
    bool _isCustomSoundEnabled;

signals:

public slots:
};

#endif // SCHEDULE_H
