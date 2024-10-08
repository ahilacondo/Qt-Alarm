#include "fileio.h"
#include "schedule.h"
#include "schedulecollection.h"
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#include <QString>
#include <QDateTime>
#include <QTime>
#include <QList>

// Constructor
FileIO::FileIO(QObject *parent) : QObject(parent)
{
}

// Load Saved Alarm Configurations
QList<Schedule *> FileIO::LoadConfig()
{
    if (this->_Settings.value("AlarmCount").isNull())
    {
        return LegacyRead();
    }

    QList<Schedule *> scheduleList;
    QString indexStr;

    for (int index = 0; index < this->_Settings.value("AlarmCount").toInt(); index++)
    {
        Schedule *sched = new Schedule(this);

        indexStr.setNum(index);

        sched->SetTime(this->_Settings.value(indexStr + "Time").toTime());
        if (sched->GetTime().isNull())
        {
            QTime reset;
            reset.setHMS(0, 0, 0, 0);
            sched->SetTime(reset);
        }

        sched->SetCustEnabled(this->_Settings.value(indexStr + "CustEnabled").toBool());
        sched->SetCust(this->_Settings.value(indexStr + "CustDate").toDate());

        sched->SetCustomSoundEnabled(this->_Settings.value(indexStr + "CustomSoundEnabled").toBool());
        sched->SetCustomSound(this->_Settings.value(indexStr + "CustomSound").toString());

        scheduleList.append(sched);
    }
    return scheduleList;
}

// Saves Modified Alarm Configurations
bool FileIO::Save(ScheduleCollection *Collection)
{
    try
    {
        QList<Schedule *> SchedList = Collection->GetScheduleList();
        Schedule *currentSche;
        int index = 0;

        this->_Settings.setValue("AlarmCount", SchedList.count());
        foreach (currentSche, SchedList)
        {
            QString IndexStr;
            IndexStr.setNum(index);
            this->_Settings.setValue(IndexStr + "Time", currentSche->GetTime());
            this->_Settings.setValue(IndexStr + "CustEnabled", currentSche->GetCustomEnabled());
            this->_Settings.setValue(IndexStr + "CustDate", currentSche->GetCustomDate());
            this->_Settings.setValue(IndexStr + "CustomSoundEnabled", currentSche->GetCustomSoundEnabled());
            this->_Settings.setValue(IndexStr + "CustomSound", currentSche->GetCustomSound());
            this->_Settings.sync();
            index++;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

//Load Playback volume
int FileIO::LoadVolume()
{
    QSettings settings;
    return settings.value("Volume").toInt();
}

//Save Playback volume
void FileIO::SaveVolume(int vol)
{
    QSettings settings;
    settings.setValue("Volume", vol);
}

//Loads Saved Alarm Items
QList<Schedule *> FileIO::LegacyRead()
{
    QList<Schedule *> convertedSche;

    for (int index = 0; index < 5; index++)
    {

        QString Index;
        Index.setNum(index);
        if (this->_Settings.value(Index + "WDEnabled").toBool())
        {
            Schedule *newSche = new Schedule;

            if (this->_Settings.value(Index + "WDTime").toTime().isNull())
            {
                QTime emptyTime;
                emptyTime.setHMS(0, 0, 0, 0);
                newSche->SetTime(emptyTime);
            }
            else
            {
                newSche->SetTime(this->_Settings.value(Index + "WDTime").toTime());
            }

            if (this->_Settings.value((Index + "CustomSoundEnabled")).isNull() == false)
            {
                newSche->SetCustomSoundEnabled(this->_Settings.value(Index + "CustomSoundEnabled").toBool());
                newSche->SetCustomSound(this->_Settings.value(Index + "CustomSound").toString());
            }
            convertedSche.append(newSche);
        }

        if (this->_Settings.value(Index + "WEEnabled").toBool())
        {
            Schedule *newSche = new Schedule;

            if (this->_Settings.value(Index + "WETime").toTime().isNull())
            {
                QTime emptyTime;
                emptyTime.setHMS(0, 0, 0, 0);
                newSche->SetTime(emptyTime);
            }
            else
            {
                newSche->SetTime(this->_Settings.value(Index + "WETime").toTime());
            }

            if (this->_Settings.value((Index + "CustomSoundEnabled")).isNull() == false)
            {
                newSche->SetCustomSoundEnabled(this->_Settings.value(Index + "CustomSoundEnabled").toBool());
                newSche->SetCustomSound(this->_Settings.value(Index + "CustomSound").toString());
            }
            convertedSche.append(newSche);
        }

        if (this->_Settings.value(Index + "CustEnabled").toBool())
        {
            Schedule *newSche = new Schedule;
            newSche->setIsCustomEnabled(true);
            if (this->_Settings.value(Index + "CustTime").toDateTime().isNull())
            {
                QTime emptyTime;
                emptyTime.setHMS(0, 0, 0, 0);
                newSche->SetTime(emptyTime);
            }
            else
            {
                QDateTime val = this->_Settings.value(Index + "CustTime").toDateTime();
                newSche->SetTime(val.time());
                newSche->SetCust(val.date());
            }
            if (this->_Settings.value((Index + "CustomSoundEnabled")).isNull() == false)
            {
                newSche->SetCustomSound(this->_Settings.value(Index + "CustomSound").toString());
                newSche->SetCustomSoundEnabled(this->_Settings.value(Index + "CustomSoundEnabled").toBool());
            }
            convertedSche.append(newSche);
        }
    }
    return convertedSche;
}
