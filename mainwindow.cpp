#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"
#include "alarm.h"
#include "fileio.h"
#include "schedulecollection.h"
#include "snooze.h"
#include <QMessageBox>
#include <QCheckBox>
#include <QLabel>
#include <QCloseEvent>
#include <QTimeEdit>
#include <QTimer>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QSlider>
#include <QSystemTrayIcon>
#include <QListWidgetItem>
#include <QCalendarWidget>

// Constructor≈
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    _Schedules = new ScheduleCollection(this);
    _Schedules->LoadSchedules();
    PopulateListWidget();
    DisablePanelIfNoSelection();

    _isMilTime = false;
    _WarnOnPm = false;
    _prevTimeWasMil = _isMilTime;
    displayTimeMode();

    TimeKeeper = new Timer(this, _Schedules);
    CurAlarm = new Alarm(this);
    TimeKeeper->StartTimer(CurAlarm);

    int Volume = FileIO::LoadVolume();
    ui->VolumeSlider->setValue(Volume <= 0 ? 50 : Volume);
    CurAlarm->SetVolume(ui->VolumeSlider->value());
    ui->listAlmBtn->button(QDialogButtonBox::Ok)->setText("&Agregar");
    ui->listAlmBtn->button(QDialogButtonBox::Cancel)->setText("&Quitar");

    ui->txtSoundPath->setText("");
    ui->CustEdit->setDate(QDate::currentDate());
    SetupClock();

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(Quit()));
    connect(ui->timeEdit, SIGNAL(editingFinished()), this, SLOT(SetTime()));
    connect(ui->listAlmBtn, SIGNAL(clicked(QAbstractButton *)), this, SLOT(AddRemoveAlarm(QAbstractButton *)));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(ShowActiveAlarm(int)));
    connect(ui->chkSounds, SIGNAL(clicked()), this, SLOT(OpenDialog()));
    connect(ui->SaveBtn, SIGNAL(clicked()), this, SLOT(SaveAll()));
    connect(ui->VolumeSlider, SIGNAL(valueChanged(int)), CurAlarm, SLOT(SetVolume(int)));
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(SetCustomDate()));
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

//Triggered on windows close
void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();

    Quit();
}

//Initiates a timer for the alarm clocks functioning
void MainWindow::SetupClock()
{
    QTimer *CurrentTime = new QTimer(this);
    connect(CurrentTime, SIGNAL(timeout()), this, SLOT(timeCheck()));
    CurrentTime->start(500);
}

//Saves User set Alarm preferences
void MainWindow::SaveAll()
{
    Schedule *Active = this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->SetCustEnabled(true);
    SetCustomDate();

    UpdateListWidget();
}

//Sets the alarm time for the specific alarm item
void MainWindow::SetTime()
{

    if (ui->listWidget->currentIndex().column() != -1)
    {
        Schedule *Active = this->_Schedules->GetSchedule(ui->listWidget->currentRow());
        Active->SetTime(ui->timeEdit->time());
        UpdateListWidget();
    }
}

// Sets alarm date for the specific alarm item
void MainWindow::SetCustomDate()
{
    if (ui->listWidget->currentIndex().column() != -1)
    {
        ui->CustEdit->setDate(ui->calendarWidget->selectedDate());
        Schedule *Active = this->_Schedules->GetSchedule(ui->listWidget->currentRow());
        Active->SetTime(ui->timeEdit->time());
        QDate CustomDate = ui->calendarWidget->selectedDate();
        Active->SetCust(CustomDate);
        if (Active->isCustomEnabled())
            UpdateListWidget();
    }
}

//Quits the application
void MainWindow::Quit()
{
    this->_Schedules->Save();
    FileIO::SaveVolume(ui->VolumeSlider->value());
    qApp->quit();
}

// Adds or Removes an alarm item
void MainWindow::AddRemoveAlarm(QAbstractButton *button)
{
    if (button->text() == "&Agregar")
    {
        Schedule *scheToAdd = new Schedule(this);
        this->_Schedules->AddSchedule(scheToAdd);
        ui->listWidget->addItem(scheToAdd->Name());
        ui->listWidget->setCurrentRow(ui->listWidget->count() - 1);
    }
    else if (button->text() == "&Quitar")
    {
        this->_lastDeletedIndex = ui->listWidget->currentRow();
        this->_Schedules->removeScheduleByIndex(ui->listWidget->currentRow());
        PopulateListWidget();
    }
}

// Shows preferences for the selected item of alarm items
void MainWindow::ShowActiveAlarm(int index)
{
    DisablePanelIfNoSelection();
    if (index == -1)
    {
        return;
    }
    Schedule *active = this->_Schedules->GetSchedule(index);
    ui->timeEdit->setTime(active->GetTime());

    ui->CustEdit->setDate(active->GetCustomDate());
    ui->txtSoundPath->setText(active->GetCustomSound());

    ui->calendarWidget->setSelectedDate(active->GetCustomDate());
}

// Checks time with the timer
void MainWindow::timeCheck()
{
    SnoozeMenuCheck();
    UpdateClock();
    if (_isMilTime != _prevTimeWasMil)
    {
        _prevTimeWasMil = _isMilTime;
        displayTimeMode();
    }
}

void MainWindow::UpdateClock()
{
    if(_isMilTime)
    {
        ui->Clock->setText(QTime::currentTime().toString("H:mm:ss"));
    }else{
        ui->Clock->setText(QTime::currentTime().toString("h:mm:ss ap"));
    }
}

// Opens the File Select Dialog to choose an audio file for alarm
void MainWindow::OpenDialog()
{
    Schedule *Active = this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->SetCustomSoundEnabled(true);

    QString AlarmLocation = QFileDialog::getOpenFileName(this, "Select Alarm File", QDir::homePath());
    Active->SetCustomSound(AlarmLocation);
    ui->txtSoundPath->setText(AlarmLocation);
}

// Opens Snooze on alarm trigger
void MainWindow::SnoozeMenuCheck()
{
    if (this->CurAlarm->isPlaying() && this->CurAlarm->canResume)
    {
        snooze *snMenu = new snooze(this, CurAlarm);
        snMenu->show();
        this->CurAlarm->canResume = false;
    }
}

// Display structure of the time and date format
void MainWindow::displayTimeMode()
{
    if (_isMilTime)
    {
        ui->timeEdit->setDisplayFormat("H:mm:ss");
        ui->CustEdit->setDisplayFormat("d MMM yyyy");
    }
    else
    {
        ui->timeEdit->setDisplayFormat("h:mm:ss ap");
        ui->CustEdit->setDisplayFormat("d MMM yyyy");
    }
}

// Populates the alarm items list
void MainWindow::PopulateListWidget()
{
    ui->listWidget->clear();
    Schedule *sche;
    foreach (sche, this->_Schedules->GetScheduleList())
    {
        ui->listWidget->addItem(sche->Name());
    }
    ui->listWidget->setCurrentRow(this->_lastDeletedIndex);
}

// Disables the window controls if not alarm item is selected
void MainWindow::DisablePanelIfNoSelection()
{
    if (ui->listWidget->currentRow() == -1)
    {

        ui->timeEdit->setEnabled(false);
        ui->CustEdit->setEnabled(false);

        ui->txtSoundPath->setText("");
    }
    else
    {
        ui->CustEdit->setEnabled(true);
        ui->timeEdit->setEnabled(true);
    }
}

// Updates the alarm items list
void MainWindow::UpdateListWidget()
{
    int index = ui->listWidget->currentRow();
    PopulateListWidget();
    ui->listWidget->setCurrentRow(index);
}
