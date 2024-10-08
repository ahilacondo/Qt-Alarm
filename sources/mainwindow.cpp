#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"
#include "alarm.h"
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

    CurAlarm->SetVolume(50);
    ui->listAlmBtn->button(QDialogButtonBox::Ok)->setText("&Agregar");
    ui->listAlmBtn->button(QDialogButtonBox::Cancel)->setText("&Quitar");

    ui->txtSoundPath->setText("");
    ui->CustEdit->setDate(QDate::currentDate());
    SetupClock();

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(Quit()));
    connect(ui->timeEdit, SIGNAL(editingFinished()), this, SLOT(SetTime()));
    connect(ui->listAlmBtn, SIGNAL(clicked(QAbstractButton*)), this, SLOT(AddRemoveAlarm(QAbstractButton*)));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(ShowActiveAlarm(int)));
    connect(ui->chkSounds, SIGNAL(clicked()), this, SLOT(OpenDialog()));
    connect(ui->SaveBtn, SIGNAL(clicked()), this, SLOT(SaveAll()));
    connect(ui->calendarWidget, SIGNAL(clicked(QDate)), this, SLOT(SetCustomDate()));
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// Se activa al cerrar la ventana
void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();

    Quit();
}

// Inicia un temporizador para el funcionamiento de las alarmas
void MainWindow::SetupClock()
{
    QTimer *CurrentTime = new QTimer(this);
    connect(CurrentTime, SIGNAL(timeout()), this, SLOT(timeCheck()));
    CurrentTime->start(500);
}

// Guarda las preferencias de la alarma establecidas por el usuario
void MainWindow::SaveAll()
{
    Schedule *Active = this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->SetCustEnabled(true);
    SetCustomDate();

    UpdateListWidget();
}

// Establece la hora de la alarma para un elemento específico
void MainWindow::SetTime()
{

    if (ui->listWidget->currentIndex().column() != -1)
    {
        Schedule *Active = this->_Schedules->GetSchedule(ui->listWidget->currentRow());
        Active->SetTime(ui->timeEdit->time());
        UpdateListWidget();
    }
}

// Establece la fecha de la alarma para un elemento específico
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

// Cierra la aplicación
void MainWindow::Quit()
{
    this->_Schedules->Save();
    qApp->quit();
}

// Agrega o quita un elemento de alarma
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

// Muestra las preferencias para el elemento seleccionado de alarmas
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

// Verifica el tiempo con el temporizador
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

// Abre el diálogo de selección de archivo para elegir un archivo de audio para la alarma
void MainWindow::OpenDialog()
{
    Schedule *Active = this->_Schedules->GetSchedule(ui->listWidget->currentRow());
    Active->SetCustomSoundEnabled(true);

    QString AlarmLocation = QFileDialog::getOpenFileName(this, "Select Alarm File", QDir::homePath());
    Active->SetCustomSound(AlarmLocation);
    ui->txtSoundPath->setText(AlarmLocation);
}

// Abre el menú de repetición al activar la alarma
void MainWindow::SnoozeMenuCheck()
{
    if (this->CurAlarm->isPlaying() && this->CurAlarm->canResume)
    {
        snooze *snMenu = new snooze(this, CurAlarm);
        snMenu->show();
        this->CurAlarm->canResume = false;
    }
}

// Muestra la estructura del formato de hora y fecha
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

// Poblamos la lista de elementos de alarma
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

// Desactiva los controles de la ventana si no hay un elemento de alarma seleccionado
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

// Actualiza la lista de elementos de alarma
void MainWindow::UpdateListWidget()
{
    int index = ui->listWidget->currentRow();
    PopulateListWidget();
    ui->listWidget->setCurrentRow(index);
}
