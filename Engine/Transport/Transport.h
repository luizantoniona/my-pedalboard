#pragma once

#include <QObject>
#include <atomic>
#include <cstdint>

namespace Engine {

class Transport : public QObject {
    Q_OBJECT
    Q_PROPERTY( State state READ state NOTIFY stateChanged )
    Q_PROPERTY( double bpm READ bpm WRITE setBpm NOTIFY bpmChanged )
    Q_PROPERTY( bool looping READ looping WRITE setLooping NOTIFY loopingChanged )

public:
    enum class State { Stopped,
                       Playing,
                       Recording };
    Q_ENUM( State )

    explicit Transport( QObject* parent = nullptr );

    State state() const;
    double bpm() const;
    bool looping() const;

    // Position in samples — readable from any thread
    uint64_t positionSamples() const;

    void setBpm( double bpm );
    void setLooping( bool looping );

public slots:
    void play();
    void stop();
    void record();
    void rewind();

    // Called exclusively from the audio thread each callback
    void advance( uint32_t frames );

signals:
    void stateChanged();
    void bpmChanged();
    void loopingChanged();
    void positionChanged( uint64_t samples );

private:
    std::atomic<State> _state{ State::Stopped };
    std::atomic<double> _bpm{ 120.0 };
    std::atomic<bool> _looping{ false };
    std::atomic<uint64_t> _position{ 0 };
};

} // namespace Engine
