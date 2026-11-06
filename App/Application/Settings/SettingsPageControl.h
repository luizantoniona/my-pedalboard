#pragma once

#include <QObject>
#include <QStringList>

namespace Engine {
class AudioEngine;
}

class SettingsPageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QStringList driverApis READ driverApis NOTIFY driverApisChanged )
    Q_PROPERTY( QStringList inputDevices READ inputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QStringList outputDevices READ outputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QStringList sampleRates READ sampleRates NOTIFY sampleRatesChanged )
    Q_PROPERTY( QStringList frameBuffers READ frameBuffers NOTIFY frameBuffersChanged )
    Q_PROPERTY( QString currentDriverApi READ currentDriverApi NOTIFY driverApisChanged )

public:
    explicit SettingsPageControl( Engine::AudioEngine& engine, QObject* parent = nullptr );

    QStringList driverApis() const;
    QStringList inputDevices() const;
    QStringList outputDevices() const;
    QStringList sampleRates() const;
    QStringList frameBuffers() const;
    QString currentDriverApi() const;

public slots:
    void setDriverApi( int index );
    void setInputDevice( int index );
    void setOutputDevice( int index );
    void setSampleRate( int index );
    void setFrameBuffer( int index );

signals:
    void driverApisChanged();
    void devicesChanged();
    void sampleRatesChanged();
    void frameBuffersChanged();

private:
    Engine::AudioEngine& _engine;
};
