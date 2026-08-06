#pragma once

#include <QJsonObject>
#include <QList>
#include <QString>

namespace Engine {

struct Preset {
    QString name;
    QJsonObject graphData; // serialized AudioGraph
};

// Manages in-memory preset list and load/save to disk.
class PresetManager {
public:
    bool loadFromFile( const QString& path );
    bool saveToFile( const QString& path ) const;

    void addPreset( Preset preset );
    void removePreset( int index );

    const QList<Preset>& presets() const;
    const Preset* preset( int index ) const;

private:
    QList<Preset> _presets;
};

} // namespace Engine
