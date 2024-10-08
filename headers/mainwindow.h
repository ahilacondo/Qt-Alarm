#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QDialogButtonBox>
#include <QListWidgetItem>
#include "timer.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    Timer *TimeKeeper;
    Alarm *CurAlarm;
    ScheduleCollection *_Schedules;

    int _lastDeletedIndex;
    bool _isMilTime;
    bool _prevTimeWasMil;
    bool _supportsTray;
    bool _WarnOnPm;

    
    void displayTimeMode();

    void closeEvent(QCloseEvent *);
    void SetupClock();
    void UpdateClock();
    void PopulateListWidget();
    void DisablePanelIfNoSelection();
    void UpdateListWidget();

private slots:
    void ShowActiveAlarm(int index);
    void AddRemoveAlarm(QAbstractButton *);
    void SetTime();
    void SaveAll();
    void SetCustomDate();
    void Quit();
    void OpenDialog();
    void SnoozeMenuCheck();
    void timeCheck();
};

#endif // MAINWINDOW_H
