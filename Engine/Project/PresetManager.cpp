#include "PresetManager.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Engine {

bool PresetManager::loadFromFile( const QString& path ) {
    QFile file( path );
    if ( !file.open( QIODevice::ReadOnly ) ) {
        qWarning() << "PresetManager: cannot open" << path;
        return false;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( file.readAll() );
    if ( !doc.isArray() )
        return false;

    _presets.clear();
    for ( const auto& entry : doc.array() ) {
        QJsonObject obj = entry.toObject();
        _presets.append( { obj[ "name" ].toString(), obj } );
    }
    return true;
}

bool PresetManager::saveToFile( const QString& path ) const {
    QJsonArray array;
    for ( const auto& preset : _presets ) {
        QJsonObject obj = preset.graphData;
        obj[ "name" ] = preset.name;
        array.append( obj );
    }

    QFile file( path );
    if ( !file.open( QIODevice::WriteOnly ) ) {
        qWarning() << "PresetManager: cannot write" << path;
        return false;
    }
    file.write( QJsonDocument( array ).toJson() );
    return true;
}

void PresetManager::addPreset( Preset preset ) {
    _presets.append( std::move( preset ) );
}

void PresetManager::removePreset( int index ) {
    if ( index >= 0 && index < _presets.size() )
        _presets.removeAt( index );
}

const QList<Preset>& PresetManager::presets() const {
    return _presets;
}

const Preset* PresetManager::preset( int index ) const {
    if ( index < 0 || index >= _presets.size() )
        return nullptr;
    return &_presets[ index ];
}

} // namespace Engine
