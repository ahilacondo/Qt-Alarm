#include "schedulecollection.h"
#include "schedule.h"
#include "fileio.h"
#include <QString>
#include <QSettings>

ScheduleCollection::ScheduleCollection(QObject *parent) :
    QObject(parent){}

//Initialize configurations for the scheduled alarm
void ScheduleCollection::LoadSchedules()
{
    FileIO *config=new FileIO(this);
    this->_Collection = config->LoadConfig();
}

//Get alarm item from scheduled alarm list
Schedule* ScheduleCollection::GetSchedule(int Index)
{
    return _Collection[Index];
}

// Saves alarm items
void ScheduleCollection::Save()
{
    FileIO * configIO=new FileIO(this);
    configIO->Save(this);
}

// Loads saved alarm items
QList<Schedule*> ScheduleCollection::GetScheduleList()
{
    return this->_Collection;
}

//Get scheduled alarms count
int ScheduleCollection::GetCount()
{
    return this->_Collection.count();
}

//adds alarms items to the scheduled alarms collection
void ScheduleCollection::AddSchedule(Schedule *scheToAdd)
{
    this->_Collection.append(scheToAdd);
}

//removes alarm item from the sheduled alarms collection
void ScheduleCollection::removeScheduleByIndex(int index)
{
    this->_Collection.removeAt(index);
}
