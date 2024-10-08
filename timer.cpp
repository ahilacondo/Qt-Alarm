#include "timer.h"
#include <QTimer>
#include <QDateTime>
#include <QDate>
#include "schedule.h"
#include "schedulecollection.h"
#include "fileio.h"

//Constructor
Timer::Timer(QObject *parent, ScheduleCollection *Collection) : QObject(parent)
{
    this->_Schedules = Collection;
}

//Initializes the timer
void Timer::StartTimer(Alarm *MainAlarm)
{
    this->_CurAlarm = MainAlarm;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(AlarmCheck()));
    timer->start(1000);
}

//Checks alarm status
void Timer::AlarmCheck()
{
    //Compare saved times with now time
    if (!this->_CurAlarm->isPlaying() && this->_CurAlarm->canResume)
    {
        Schedule *cur_sche;
        foreach (cur_sche, this->_Schedules->GetScheduleList())
        {
            if (cur_sche->GetCustomSoundEnabled())
            {
                this->_CurAlarm->SetCustomPath(cur_sche->GetCustomSound());
            }

            QDateTime RightNow = QDateTime::currentDateTime();
            bool soundAlarm = false;

            if (cur_sche->GetCustomEnabled() && cur_sche->GetCustomDate() == RightNow.date() &&
                cur_sche->GetTime().minute() == RightNow.time().minute() && cur_sche->GetTime().hour() == RightNow.time().hour())
            {
                soundAlarm = true;
            }

            if (soundAlarm)
            {
                this->_CurAlarm->Start(cur_sche->GetCustomSoundEnabled());
            }
        }
    }
}
