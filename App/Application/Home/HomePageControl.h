#pragma once

#include <QObject>
#include <QString>

namespace Engine {
class AudioEngine;
}

class HomePageControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( bool running READ isRunning NOTIFY streamStateChanged )
    Q_PROPERTY( QString latencyText READ latencyText NOTIFY streamStateChanged )
    Q_PROPERTY( QString streamText READ streamText NOTIFY streamStateChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

public:
    explicit HomePageControl( Engine::AudioEngine& engine, QObject* parent = nullptr );

    bool isRunning() const;
    QString latencyText() const;
    QString streamText() const;
    QString lastError() const;

public slots:
    void start();
    void stop();
    void toggle();

signals:
    void streamStateChanged();
    void lastErrorChanged();

private:
    Engine::AudioEngine& _engine;
    QString _lastError;
};
