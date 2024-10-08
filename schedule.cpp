#include "schedule.h"
#include <QDateTime>
#include <QTime>
#include <QString>

//Constructor
Schedule::Schedule(QObject *parent) : QObject(parent)
{

    this->SetCustEnabled(false);
    this->SetTime(QTime::fromMSecsSinceStartOfDay(0));
    this->_CustomAlarm = QDate::currentDate();
    this->SetCustomSound("");
    this->SetCustomSoundEnabled(false);
}

// Sets Custom Date Enabled Flag
void Schedule::SetCustEnabled(bool isEnabled)
{
    this->setIsCustomEnabled(isEnabled);
}

//Sets Custom Sounds enabled flag
void Schedule::SetCustomSoundEnabled(bool isEnabled)
{
    this->_isCustomSoundEnabled = isEnabled;
}

//Schedules alarm time
void Schedule::SetTime(QTime time)
{
    this->_AlarmTime = time;
}

//Get scheduled alarm time
QTime Schedule::GetTime()
{
    return this->_AlarmTime;
}

//Sets scheduled alarm date
void Schedule::SetCust(QDate Date)
{
    this->_CustomAlarm = Date;
}

//Sets custom audio file path
void Schedule::SetCustomSound(QString SoundPath)
{
    this->_CustomSound = SoundPath;
}

//Gets custom date schedule enabled flag
bool Schedule::GetCustomEnabled()
{
    return this->isCustomEnabled();
}

//Gets custom audio enabled flag
bool Schedule::GetCustomSoundEnabled()
{
    return this->_isCustomSoundEnabled;
}

//Gets custom date value
QDate Schedule::GetCustomDate()
{
    return this->_CustomAlarm;
}

//Gets custom sound path
QString Schedule::GetCustomSound()
{
    return this->_CustomSound;
}

//get custom date enabled flag
bool Schedule::isCustomEnabled() const
{
    return _isCustomEnabled;
}

//sets custom date enabled flag
void Schedule::setIsCustomEnabled(bool isCustomEnabled)
{
    _isCustomEnabled = isCustomEnabled;
}

//gets custom sound enabled flag
bool Schedule::isCustomSoundEnabled() const
{
    return _isCustomSoundEnabled;
}

//sets custom sound enabled flag
void Schedule::setIsCustomSoundEnabled(bool isCustomSoundEnabled)
{
    _isCustomSoundEnabled = isCustomSoundEnabled;
}

//Gets generated alarm name
QString Schedule::Name()
{
    QString name;
    name = this->GetTime().toString() + "  ";
    if (this->isCustomEnabled())
        name.append("  " + this->_CustomAlarm.toString());

    return name;
}
