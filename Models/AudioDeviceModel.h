#pragma once

#include <QObject>
#include <QStringList>

#include <Engine/Audio/AudioEngine.h>

namespace Models {

// Reflects the state of audio input/output devices for the UI layer.
// Updated automatically when AudioEngine reports device changes.
class AudioDeviceModel : public QObject {
    Q_OBJECT
    Q_PROPERTY( QStringList inputDevices READ inputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QStringList outputDevices READ outputDevices NOTIFY devicesChanged )
    Q_PROPERTY( int currentInput READ currentInput WRITE setCurrentInput NOTIFY currentInputChanged )
    Q_PROPERTY( int currentOutput READ currentOutput WRITE setCurrentOutput NOTIFY currentOutputChanged )

public:
    explicit AudioDeviceModel( Engine::AudioEngine& engine, QObject* parent = nullptr );

    QStringList inputDevices() const;
    QStringList outputDevices() const;
    int currentInput() const;
    int currentOutput() const;

    void setCurrentInput( int index );
    void setCurrentOutput( int index );

signals:
    void devicesChanged();
    void currentInputChanged();
    void currentOutputChanged();

private:
    Engine::AudioEngine& _engine;
    int _currentInput{ -1 };
    int _currentOutput{ -1 };
};

} // namespace Models
