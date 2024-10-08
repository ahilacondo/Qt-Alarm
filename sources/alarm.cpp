#include "alarm.h"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QString>
#include <QDir>
#include <QTimer>

// Constructor
Alarm::Alarm(QObject *parent) : QObject(parent)
{
    media = new QMediaPlayer(this);

    this->_DefaultPath = QDir::tempPath() + "/QTalarm.ogg";
    this->_isPlaying = false;
    this->_Pause = new QTimer(this);
    this->canResume = true;

    connect(this->_Pause, &QTimer::timeout, this, &Alarm::Resume);
}

// Inicia la reproducción de la alarma cuando se activa
void Alarm::Start(bool useCustom)
{
    if (useCustom)
    {
        media->setSource(QUrl::fromLocalFile(this->_CustPath));
        this->UsingCustomPath = true;
    }
    else
    {
        media->setSource(QUrl::fromLocalFile(this->_DefaultPath));
        this->UsingCustomPath = false;
    }
    media->play();
    connect(media, &QMediaPlayer::mediaStatusChanged, this, &Alarm::RepeatAllTheThings);
    this->_isPlaying = true;
}

// Detiene la reproducción de audio de la alarma
void Alarm::Stop()
{
    media->stop();
    this->_Pause->start(60000);
    this->_isPlaying = false;
}

void Alarm::RepeatAllTheThings(QMediaPlayer::MediaStatus state)
{
    if (state == QMediaPlayer::EndOfMedia)
    {
        if (this->UsingCustomPath)
        {
            media->setSource(QUrl::fromLocalFile(this->_CustPath));
        }
        else
        {
            media->setSource(QUrl::fromLocalFile(this->_DefaultPath));
        }
        media->play();
    }
}

// Devuelve si el audio se está reproduciendo
bool Alarm::isPlaying()
{
    return this->_isPlaying;
}

// Establece la ruta del archivo de audio personalizado
void Alarm::SetCustomPath(QString CustPath)
{
    this->_CustPath = CustPath;
}

// Reanuda la reproducción de audio al activar el modo de posponer
void Alarm::Resume()
{
    this->canResume = true;
    this->_Pause->stop();
}

// Establece el volumen de reproducción de audio
void Alarm::SetVolume(int Volume)
{
    if (Volume < 0) Volume = 0;
    if (Volume > 100) Volume = 100;
    media->setAudioOutput(new QAudioOutput);
    media->audioOutput()->setVolume(Volume / 100.0);
}
