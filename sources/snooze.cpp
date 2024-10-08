#include <QTimer>
#include <QTime>
#include "snooze.h"
#include "alarm.h"
#include "ui_snooze.h"

//constructor
snooze::snooze(QWidget *parent, Alarm *curAlarm) : QDialog(parent),
                                                   ui(new Ui::snooze)
{
    ui->setupUi(this);
    this->_curAlarm = curAlarm;
    this->_snoozeTimer = new QTimer(this);
    this->isDismissed = false;
    SetupClock();

    connect(ui->snzBtn, SIGNAL(clicked()), SLOT(SnoozeClicked()));
    connect(ui->DismissBtn, SIGNAL(clicked()), SLOT(DismissClicked()));
    connect(this->_snoozeTimer, SIGNAL(timeout()), SLOT(timerOut()));
}

//destructor
snooze::~snooze()
{
    delete ui;
}

//Setups the alarm time clock
void snooze::SetupClock()
{
    UpdateClock();
    QTimer *CurrentTime = new QTimer(this);
    connect(CurrentTime, SIGNAL(timeout()), this, SLOT(UpdateClock()));
    CurrentTime->start(500);
}

//Updates the alarm time label
void snooze::UpdateClock()
{
    ui->time->setText(QTime::currentTime().toString("h:mm:ss ap"));
}

//Snoozes alarm on snooze button click
void snooze::SnoozeClicked()
{
    this->_snoozeTimer->start(300000); //Five Minutes
    this->_curAlarm->Stop();
}

//Dismisses alarm on dismiss button click
void snooze::DismissClicked()
{
    this->_snoozeTimer->stop();
    this->_curAlarm->Stop();
    this->hide();
    this->~snooze();
}

//Starts snooze alarm on snooze timeout
void snooze::timerOut()
{
    this->_curAlarm->Start(this->_curAlarm->UsingCustomPath);
    this->_curAlarm->canResume = false;
}
